/**
  ******************************************************************************
  * @file    stm32_eval.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  * 
  * 
  * 
  *
  * 
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#include "my_platform.h"

/** @defgroup STM32 Private_Variables
  * @{
  */ 
	
/*LED*/
LED_TypeDef			LED[LEDn]				=	{LED1,								LED2,									LED3,									LED4};
GPIO_TypeDef*		LED_PORT[LEDn] 	= {EVAL_LED1_GPIO_PORT, EVAL_LED2_GPIO_PORT, 	EVAL_LED3_GPIO_PORT, 	EVAL_LED4_GPIO_PORT	};
const uint32_t	LED_CLK[LEDn] 	= {EVAL_LED1_CLK, 			EVAL_LED2_CLK, 				EVAL_LED3_CLK, 				EVAL_LED4_CLK				};
const uint16_t 	LED_PIN[LEDn] 	=	{EVAL_LED1_PIN, 			EVAL_LED2_PIN, 				EVAL_LED3_PIN, 				EVAL_LED4_PIN				};
	
/**
  * @}
  */ 
void STM_EVAL_BUZZERInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(EVAL_BUZZER_CLK, ENABLE);			//使能端口时钟
	
	GPIO_InitStructure.GPIO_Pin = EVAL_BUZZER_PIN;				//端口配置
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//IO口速度为50MHz
	
	GPIO_Init(EVAL_BUZZER_GPIO_PORT, &GPIO_InitStructure);//根据设定参数初始化	
	
	u_printf(INFO,"BUZZER Init is Successful.");
	Write_BUZZER_Status(0);	//关闭BUZZER
}

//Write LEDn Status
void Write_BUZZER_Status(u8 Status)
{
	if(Status)
	{//open 1
		GPIO_SetBits(EVAL_BUZZER_GPIO_PORT,EVAL_BUZZER_PIN);
//		u_printf(DBG,"BUZZER is Open.");
	}
	else
	{//close 0
		GPIO_ResetBits(EVAL_BUZZER_GPIO_PORT,EVAL_BUZZER_PIN);
//		u_printf(DBG,"BUZZER is Close.");
	}
}	

void BUZZER_Beep(u16 time)
{
	Write_BUZZER_Status(1);
	Sleep(time);
	Write_BUZZER_Status(0);
}
//Read LEDn Status
uint8_t Read_LEDn_Status(LED_TypeDef x)
{
	uint8_t return_val = 0;
	if(GPIO_ReadOutputDataBit(LED_PORT[x],LED_PIN[x]) == 0)//IO = 0,LED Open
		return_val = 1;
	else
		return_val = 0;
	return (return_val);
}

//Write LEDn Status
void Write_LEDn_Status(LED_TypeDef x,u8 Status)
{
	if(Status)
	{//open 1
		GPIO_SetBits(LED_PORT[x],LED_PIN[x]);
//		u_printf(DBG,"LED%d is Open.",x+1);
	}
	else
	{//close 0
		
		GPIO_ResetBits(LED_PORT[x],LED_PIN[x]);
//		u_printf(DBG,"LED%d is Close.",x+1);
	}
}	

//LEDn Init
static void STM_EVAL_LEDn_Init(LED_TypeDef LED)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_CLK[LED], ENABLE);				//使能端口时钟
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN[LED];					//端口配置
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	
	GPIO_Init(LED_PORT[LED], &GPIO_InitStructure);			//根据设定参数初始化	
	u_printf(INFO,"LED%d Init is Successful.",LED+1);
	Write_LEDn_Status(LED,0);	//关闭LED
}

//Init All LED
void STM_EVAL_LED_Init(void)
{
	u8 i = 0;
	for(i = 0;i < LEDn;i++)//初始化所有的LED
	{
		STM_EVAL_LEDn_Init(LED[i]);
	}
}
