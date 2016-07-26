/**
  ******************************************************************************
  * @file    usb_lib.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    28-August-2012
  * @brief   USB library include files
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_LIB_H
#define __USB_LIB_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_type.h"
#include "usb_regs.h"
#include "usb_def.h"
#include "usb_core.h"
#include "usb_init.h"
#include "usb_sil.h"
#include "usb_mem.h"
#include "usb_int.h"

#define PMAToUserBufferCopy(A, B, C) {\
  uint32_t n = (C + 1) >> 1;/* /2*/\
  uint32_t i;\
  uint32_t *pdwVal;\
  pdwVal = (uint32_t *)(B * 2 + PMAAddr);\
  for (i = n; i != 0; i--)\
  {\
    *(uint16_t*)A++ = *pdwVal++;\
    A++;\
  }\
}

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif /* __USB_LIB_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
