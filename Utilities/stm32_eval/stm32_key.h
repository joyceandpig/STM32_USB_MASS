/**
  ******************************************************************************
  * @�ļ�  stm32_key.h
  * @����	 fengguangrong 
  * @�汾  V1.0.0
  * @����  13/4/2016
  * @˵��  
  ******************************************************************************
  * @��Ȩ
  *
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef		__STM32_KEY_H__
#define		__STM32_KEY_H__

/* Includes ------------------------------------------------------------------*/
#include "my_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif


void user_key_init(void);
u8 update_key(void) ;


#ifdef __cplusplus
}

#endif

#endif/*__STM32_KEY_H__*/


/******************* (C) COPYRIGHT 2013 SUNSHEEN Technology *****END OF FILE****/

