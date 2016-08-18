/*
操作对象为一24C02
*/

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "I2C.h"
#include <stdio.h>

#define AT24C01A		//24C01A,I2C时序和往后的24C02一样

#define EEPROM_ADDR		0xA0
#define I2C_PAGESIZE	8		//24C02页缓冲是8个


void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
    

	/* PB6,7 SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //设置管脚为复用功能开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);          //I2C接口使用的GPIO管脚初始化
	
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;         //设置I2C接口模式 
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //设置I2C接口的高低电平周期
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;          //设置I2C接口的主机地址
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        //设置是否开启ACK响应          
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;        //100K速度
    
	I2C_Cmd(I2C1, ENABLE);                            //使能I2C接口
	I2C_Init(I2C1, &I2C_InitStructure);               //I2C接口初始化
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);              //使能I2C接口响应

}

/***************************************************
**函数名:I2C_ReadS
**功能:读取24C02多个字节
**注意事项:24C02是256字节,8位地址,A0-A2固定为0,从器件地址为EEPROM_ADDR
***************************************************/
void I2C_ReadS_24C(u8 addr ,u8* pBuffer,u16 no)
{
    if(no==0)	return;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	/* 发送起始位 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

#ifdef AT24C01A	
    /*发送器件地址(写)*/
    I2C_Send7bitAddress(I2C1,  EEPROM_ADDR, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送地址*/
	I2C_SendData(I2C1, addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
		
	/*起始位*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*器件读*/
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	
#else	
	/*发送器件地址(读)24C02*/
	I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
#endif
	
    while (no)
    {
		if(no==1)
		{
     		I2C_AcknowledgeConfig(I2C1, DISABLE);	//最后一位后要关闭应答的
    		I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位
		}
	    
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
	    *pBuffer = I2C_ReceiveData(I2C1);
	    pBuffer++;
	    /* Decrement the read bytes counter */
	    no--;
    }
	//再次允许应答模式
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/****************************************************
**函数名:I2C_Standby_24C
**功能:24C是否准备好再写入的判断
**注意事项:本函数可以理解为:判忙
****************************************************/
void I2C_Standby_24C(void)      
{
  vu16 SR1_Tmp;
  do
  {
    /*起始位*/
    I2C_GenerateSTART(I2C1, ENABLE);
    /*读SR1*/
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /*器件地址(写)*/
    #ifdef AT24C01A
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	#else
	I2C_Send7bitAddress(I2C1, 0, I2C_Direction_Transmitter);
	#endif
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  /**/
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
  /*停止位*/    
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/*************************************************
**函数名:I2C_ByteWrite_24C
**功能:写一个字节
**注意事项:字写入同样需要调用忙判断
*************************************************/
void I2C_ByteWrite_24C(u8 addr,u8 dat)
{
  /* 起始位 */
  	I2C_GenerateSTART(I2C1, ENABLE);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

#ifdef AT24C01A
  	/* 发送器件地址(写)*/
  	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  	/*发送地址*/
  	I2C_SendData(I2C1, addr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#else	
	I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Transmitter);
 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#endif

	/* 写一个字节*/
  	I2C_SendData(I2C1, dat); 
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  	/* 停止位*/
  	I2C_GenerateSTOP(I2C1, ENABLE);
  
  	I2C_Standby_24C();
}

/*************************************************
**函数名:I2C_PageWrite_24C
**功能:写入一页(以内)
**注意事项:此函数供群写入调用
*************************************************/
void I2C_PageWrite_24C(u8 addr,u8* pBuffer, u8 no)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	/*起始位*/
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

#ifdef AT24C01A
	/*器件地址(写)*/
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	/*写地址值*/
	I2C_SendData(I2C1, addr);
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

#else
	I2C_Send7bitAddress(I2C1, addr<<1, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
#endif
	while(no--)  
	{
	  I2C_SendData(I2C1, *pBuffer); 
	  pBuffer++; 
	  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	/*停止位*/
	I2C_GenerateSTOP(I2C1, ENABLE);
}



/*************************************************
**函数名:I2C_WriteS_24C
**功能:页写入24C
**注意事项:24C02最多允许一次写入8个字节
*************************************************/
void I2C_WriteS_24C(u8 addr,u8* pBuffer,  u16 no)
{
	u8 temp;

	//先把页不对齐的部分写入
	temp= addr % I2C_PAGESIZE;
	if(temp)
	{
		temp=I2C_PAGESIZE-temp;
		I2C_PageWrite_24C(addr,pBuffer,temp);//将页不对齐的字节写入EEPROM
		no-=temp;
		addr+=temp;
		pBuffer+=temp;
		I2C_Standby_24C();//判断EEPROM是否忙
	}
	//从页对齐开始写
	while(no)
	{
		if(no>=I2C_PAGESIZE)
		{
			I2C_PageWrite_24C(addr,pBuffer,I2C_PAGESIZE);//将页对齐的字节写入EEPROM
			no-=I2C_PAGESIZE;
			addr+=I2C_PAGESIZE;
			pBuffer+=I2C_PAGESIZE;
			I2C_Standby_24C();//判断EEPROM是否忙
		}
		else
		{
			I2C_PageWrite_24C(addr,pBuffer, no);
			no=0;
			I2C_Standby_24C();
		}
	}
}

void I2C_Test(void)
{
	u16 i;
	u8 I2c_Buf[256];
	
    printf("写入的数据\n\r");
    
	//填充缓冲
	for(i=0;i<=255;i++)
    {   
        I2c_Buf[i]=i+1;
        printf("0x%x\t",I2c_Buf[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
    }
    printf("\n\r");
    
	I2C_WriteS_24C(0,I2c_Buf,256);	//将I2C_Buf中顺序递增的数据写入EERPOM中  

	
	//清缓冲
	for(i=0;i<=255;i++)
    {   
		I2c_Buf[i]=0;
    }
    
    printf("读出的数据\n\r");
	I2C_ReadS_24C(0,I2c_Buf,256);//将EEPROM读出数据顺序保持到I2C_Buf中  

    //将I2C_Buf中的数据通过串口打印
	for(i=0;i<256;i++)
	{	
		if(I2c_Buf[i]!=(u8)(i+1))
		{
            printf("错误:I2C EEPROM写入与读出的数据不一致\n\r");
			while(1);
		}
        printf("0x%X\t", I2c_Buf[i]);
        if(i%16 == 15)
        {
            printf("\n\r");
        }
	}
}
