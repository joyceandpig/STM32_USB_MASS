/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32_fsmc_nor_flash.h
* Author             : MCD Application Team
* Version            : V0.0.1
* Date               : 06/13/2016
* Description        : Header for stm32_fsmc_nor_flash.c file.


//驱动芯片SST 39VF160 2MB NOR FLASH
//原理图参考《神舟III号V1.1 开发板》相关资料
//接线：FSMC BANK1的子板块2
//数据宽度:16Bit 
//使用NOR FLASH运行程序时，必要的配置文件必须加载到内部Flash方能正常运行！！！

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_FSMC_NOR_H
#define __STM32_FSMC_NOR_H



/* Includes ------------------------------------------------------------------*/
#include "my_platform.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  u16 Manufacturer_Code;
  u16 Device_Code1;
}NOR_IDTypeDef;

/* NOR Status */
typedef enum
{
  NOR_SUCCESS = 0,
  NOR_ONGOING,
  NOR_ERROR,
  NOR_TIMEOUT
}NOR_Status;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern NOR_IDTypeDef NOR_ID;
void FSMC_NOR_Init(void);
void FSMC_NOR_ReadID(NOR_IDTypeDef* NOR_ID);
u16 FSMC_NOR_Read_Device_Size(void);
NOR_Status FSMC_NOR_EraseBlock(u32 BlockAddr);
NOR_Status FSMC_NOR_EraseChip(void);
NOR_Status FSMC_NOR_WriteHalfWord(u32 WriteAddr, u16 Data);
NOR_Status FSMC_NOR_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
NOR_Status FSMC_NOR_ProgramBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
u16 FSMC_NOR_ReadHalfWord(u32 ReadAddr);
void FSMC_NOR_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);
NOR_Status FSMC_NOR_ReturnToReadMode(void);
NOR_Status FSMC_NOR_Reset(void);
NOR_Status FSMC_NOR_GetStatus(u32 Timeout);

#endif /* __STM32_FSMC_NOR_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
