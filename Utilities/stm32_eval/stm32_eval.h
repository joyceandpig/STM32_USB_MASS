#ifndef __STM32_EVAL_H
#define __STM32_EVAL_H

#ifdef __cplusplus
 extern "C" { 
#endif

	
#define 	NOR_FLASH
#define 	EXT_SRAM
#define 	NAND_FLASH
#define 	SD_CARD

 
/* Includes ------------------------------------------------------------------*/
#include "stm32_uart.h"
#include "stm32_key.h"

	 
#ifdef NOR_FLASH
	#include "stm32_fsmc_nor_flash.h"
#endif

#ifdef EXT_SRAM
	#include "stm32_fsmc_sram.h"
#endif

#ifdef NAND_FLASH
	#include "stm32_fsmc_nand.h"
#endif

struct IO_PORT 
{
	uint32_t RCC_APB2Periph;
	GPIO_TypeDef *GPIO_x;                  
	unsigned short GPIO_pin; 
}; 

/****************************************************/
/** @addtogroup STM32_EVAL_LED
  * @{
  */
#define EVAL_BUZZER_CLK                 RCC_APB2Periph_GPIOB
#define EVAL_BUZZER_PIN                 GPIO_Pin_11
#define EVAL_BUZZER_GPIO_PORT           GPIOB

void Write_BUZZER_Status(u8 Status);//Write LEDn Status
void STM_EVAL_BUZZERInit(void);
void BUZZER_Beep(u16 time);
/**
  * @}
  */
	
/** @addtogroup STM32_EVAL_LED
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3  
} LED_TypeDef; 

#define LEDn	4
extern LED_TypeDef	LED[LEDn];
/**
 * @brief Definition for LED1
 */ 
#define EVAL_LED1_CLK                 RCC_APB2Periph_GPIOB
#define EVAL_LED1_PIN                 GPIO_Pin_6
#define EVAL_LED1_GPIO_PORT           GPIOF
/**
 * @brief Definition for LED2
 */ 
#define EVAL_LED2_CLK                 RCC_APB2Periph_GPIOB
#define EVAL_LED2_PIN                 GPIO_Pin_7
#define EVAL_LED2_GPIO_PORT           GPIOF
/**
 * @brief Definition for LED3
 */ 
#define EVAL_LED3_CLK                 RCC_APB2Periph_GPIOB
#define EVAL_LED3_PIN                 GPIO_Pin_8
#define EVAL_LED3_GPIO_PORT           GPIOF
/**
 * @brief Definition for LED4
 */ 
#define EVAL_LED4_CLK                 RCC_APB2Periph_GPIOB
#define EVAL_LED4_PIN                 GPIO_Pin_9
#define EVAL_LED4_GPIO_PORT           GPIOF

extern LED_TypeDef			LED[LEDn];
extern GPIO_TypeDef*		LED_PORT[LEDn];
extern const uint32_t		LED_CLK[LEDn] ;
extern const uint16_t 	LED_PIN[LEDn] ;

void Write_LEDn_Status(LED_TypeDef x,u8 Status);//Write LEDn Status
uint8_t Read_LEDn_Status(LED_TypeDef x);//Read LEDn Status
static void STM_EVAL_LEDn_Init(LED_TypeDef LED);//初始化一个LED
void STM_EVAL_LED_Init(void);//初始化全部LED
/**
  * @}
  */ 
/**
  * @}
  */
	
void Timer2_Init(void);
void SoftReset(void);//软件复位
#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


