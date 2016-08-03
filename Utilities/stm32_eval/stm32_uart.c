#include "my_platform.h"

/*USART*/
USART_InitTypeDef 	\
USART_COM1 ={	57600,	\
							USART_WordLength_8b,						\
							USART_StopBits_1,								\
							USART_Parity_No,								\
							USART_Mode_Rx | USART_Mode_Tx,	\
							USART_HardwareFlowControl_None	\
						};/*COM1 参数配置*/
USART_InitTypeDef 	\
USART_COM2 ={	115200,	\
							USART_WordLength_8b,						\
							USART_StopBits_1,								\
							USART_Parity_No,								\
							USART_Mode_Rx | USART_Mode_Tx,	\
							USART_HardwareFlowControl_None	\
						};/*COM2 参数配置*/

USART_TypeDef* 	COM_USART[COMn] 				= 	{EVAL_COM1, 							EVAL_COM2}; 
GPIO_TypeDef* 	COM_TX_PORT[COMn] 			= 	{EVAL_COM1_TX_GPIO_PORT, 	EVAL_COM2_TX_GPIO_PORT};
GPIO_TypeDef* 	COM_RX_PORT[COMn] 			= 	{EVAL_COM1_RX_GPIO_PORT, 	EVAL_COM2_RX_GPIO_PORT};
const uint32_t 	COM_USART_CLK[COMn] 		= 	{EVAL_COM1_CLK, 					EVAL_COM2_CLK};
const uint32_t 	COM_TX_PORT_CLK[COMn] 	= 	{EVAL_COM1_TX_GPIO_CLK, 	EVAL_COM2_TX_GPIO_CLK};
const uint32_t 	COM_RX_PORT_CLK[COMn] 	= 	{EVAL_COM1_RX_GPIO_CLK, 	EVAL_COM2_RX_GPIO_CLK};
const uint16_t 	COM_TX_PIN[COMn] 				= 	{EVAL_COM1_TX_PIN, 				EVAL_COM2_TX_PIN};
const uint16_t 	COM_RX_PIN[COMn] 				= 	{EVAL_COM1_RX_PIN, 				EVAL_COM2_RX_PIN};
const uint16_t 	COM_IRQn[COMn]					=		{USART1_IRQn,							USART2_IRQn};
const uint16_t 	COM_IRQn_Priority[COMn]	=		{USART1_IRQn_Priority,		USART2_IRQn_Priority};
 
#include "stdio.h"
/*加入以下代码,支持printf函数,而不需要选择use MicroLIB	*/  
#if defined(__MICROLIB)
	#warning	"Use MicroLIB Is Enable! "
#else
//	#warning	"Use MicroLIB Is Disable! "            
	//标准库需要的支持函数                 
	struct __FILE 
	{ 
		int handle; 

	}; 

	FILE __stdout;       
	//定义_sys_exit()以避免使用半主机模式    
	void _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{ 
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = (u8) ch;    
		return ch;
	}
#endif 
	

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  if (COM == COM1)
  {
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
  }
  else
  {
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

	//NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = COM_IRQn[COM];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = COM_IRQn_Priority[COM] ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
	
	USART_ITConfig(COM_USART[COM], USART_IT_RXNE, ENABLE);//开启串口接受中断  
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
#if 0
	u_printf(INFO,"Serial Port COM%d Init is Successful.\r\n \tBaudRate[%ld],DataBits[%s],Parity[%s],StopBits[%s],FlowControl[%s]",COM+1,\
						USART_InitStruct->USART_BaudRate,\
						(USART_InitStruct->USART_WordLength == USART_WordLength_8b)?"8Bit":"9Bit",\
						(USART_InitStruct->USART_Parity == USART_Parity_Odd)?"Odd":(USART_InitStruct->USART_Parity == USART_Parity_Even)?"Even":"None",	\
						(USART_InitStruct->USART_StopBits == USART_StopBits_1)?"1":(USART_InitStruct->USART_StopBits == USART_StopBits_1_5)?"1.5":(USART_InitStruct->USART_StopBits == USART_StopBits_2)?"2":"0.5",\
						(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_RTS)?"RTS":(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_CTS)?"CTS":\
						(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS)?"RTS&CTS":"None"\
						);
#endif

}

void SoftReset(void)
{//软件复位
	u_printf(WARN,"System well Reset.\n");
	NVIC_SystemReset();
  __DSB();  
	while (1);
}

#define USART_RECV_LEN  			4096  	//定义最大接收字节数 512
char USART_RX_BUF[USART_RECV_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
u16 USART_Read_Data(char *buf)
{
	u16 ret;
	if(USART_RX_STA & 0x8000)
	{
		ret = USART_RX_STA&0x3fff;
		strcpy(buf,(const char *)&USART_RX_BUF);
		USART_RX_STA = 0x0000;
		memset(USART_RX_BUF,0,sizeof(char)*USART_RECV_LEN);//清空内存			
	}
	else
		ret =0;
	
	return ret;
}
u8 CMD_FLAG = 0;	
static void Recv_Process(void)
{
	s8 Res;
	static u8 EscFlag = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);

		Res =USART_ReceiveData(USART1);	//读取接收到的数据	
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(EscFlag == 0)
			{//没收到过转义字符
				if(Res >= 32 && Res <=127)
				{//有效字符							
					
					USART_SendData(USART1,Res);	//printf("%c",Res);	
					
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_RECV_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
				}					
				else
				{//控制字符
					if(Res == 0x0d)
					{//回车						
						if((USART_RX_STA&0X3FFF) != 0)
							USART_RX_STA |= 0x8000;	//接收完成了
						printf("\r\n[sunsheen@Terminal]");		

						CMD_FLAG = 1;					 
					}									
					else if(Res == 0x8)
					{								
						if(USART_RX_STA == 0)
							USART_RX_STA = 0;
						else
						{
							printf("%c\x1b[\xff\x50",Res);	//回删一个字符
							USART_RX_STA--;
						}
						USART_RX_BUF[USART_RX_STA&0X3FFF]=0 ;					
					}
					else if(Res == 0x1b)//ESC
					{//接收到转义字符
						EscFlag =1;
					}
				}
			}
			else
			{//转义字符  上下左右 "\[A"
				static u8 i =0;
				if(++i == 2)
				{
					i = 0;
					EscFlag = 0;
				}				
			}					
		}			
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
		{//先读SR 再读DR 方可清除溢出标志位
			USART_ClearFlag(USART1,USART_FLAG_ORE);        //读SR
			Res = USART_ReceiveData(USART1);               //读DR
			u_printf(WARN,"COM1 Receive is Overflow!");
		}			
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
	{//先读SR 再读DR 方可清除溢出标志位
		USART_ClearFlag(USART1,USART_FLAG_ORE);        //读SR
		Res = USART_ReceiveData(USART1);               //读DR
		u_printf(WARN,"COM1 Receive is Overflow!");
	}
} 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{  
	u8 Res;
#ifdef	OS_UCOS
	OSIntEnter();  
#endif  
#ifndef USMART
	Recv_Process();
#else
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(4096-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#endif
	
#ifdef	OS_UCOS
	OSIntExit();  	
#endif  	
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
#ifdef	OS_UCOS
	OSIntEnter();   
#endif  	
	
	u_printf(ENTER);//进入串口2中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		
	}	
	u_printf(EXIT);//退出串口2中断
	
#ifdef	OS_UCOS
	OSIntExit(); 
#endif  	
}
	
