#ifndef __STM32_UART_H
#define __STM32_UART_H


#ifdef __cplusplus
 extern "C" { 
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include "my_platform.h"
 
/** @addtogroup STM3210E_EVAL_LOW_LEVEL_COM
  * @{
  */
typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef; 

#define COMn                             2

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM1_IRQn                   USART1_IRQn
#define EVAL_COM1_IRQn_Priority          USART1_IRQn_Priority

extern 	USART_InitTypeDef	USART_COM1;
/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOA
#define EVAL_COM2_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM2_RX_PIN                 GPIO_Pin_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOA
#define EVAL_COM2_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM2_IRQn                   USART2_IRQn
#define EVAL_COM2_IRQn_Priority          USART2_IRQn_Priority

extern 	USART_InitTypeDef	USART_COM2;
/**
  * @}
  */ 

/**
  * @}
  */ 

u16 USART_Read_Data(char *buf);//¶ÁÈ¡Êý¾Ý
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */  

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
