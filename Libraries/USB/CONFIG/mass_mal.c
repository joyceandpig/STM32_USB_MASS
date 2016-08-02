/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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


/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "mass_mal.h"
#include "w25qxx.h"
#include "stm32_fsmc_nand.h"
#include "sdcard.h"
#include "stdio.h"
#include "msd.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
long long Mass_Memory_Size[MAX_LUN+1];
u32 Mass_Block_Size[MAX_LUN+1];
u32 Mass_Block_Count[MAX_LUN+1];

SD_CardInfo SDCardInfo;
__IO uint32_t Status = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{ 
	u16 Status=MAL_OK;   	  
	switch (lun)         	 
	{
		case MAL_SPIFLASH:		
			Status = W25QXX_Init();	
			printf("\nspiflash init ok!\n");
			break;			   
		case MAL_NANDFLASH:	
			FlashInit();
			printf("\nNandflash init ok!\n");
			break;		  
		case MAL_SD:
			#if 1
				Status = SD_Init();  //SD卡接口初始化
				Status = SD_GetCardInfo(&SDCardInfo); //获取SD卡信息
				Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
				Status = SD_EnableWideBusOperation(SDIO_BusWide_1b);//设置SDIO接口数据宽度
				Status = SD_SetDeviceMode(SD_DMA_MODE);//设置工作模式
			#else
				MSD_Init();
			#endif
			printf("\nSDCARD init ok!\n\n");
			break;
		default:
			return MAL_FAIL;
	}
	return Status;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint64_t Memory_Offset, uint32_t *Writebuff,  uint32_t nandlen, uint16_t Transfer_Length)
{
	u8 STA;
	
	switch (lun)		//这里,根据lun的值确定所要操作的磁盘
	{
		case MAL_SPIFLASH:		 	//磁盘0为 SPI FLASH盘	
			STA=0;
//			printf("\nWrite SpiFlash\n");	
//			printf("\nSpiWrite mem_offset = %d, len = %d\n",(int)Memory_Offset,Transfer_Length);
			W25QXX_Write((u8*)Writebuff, Memory_Offset, Transfer_Length);   		  
			break; 
		case MAL_NANDFLASH:			//磁盘1为SD卡	
			STA=0;		
//			printf("\nWrite NandFlash\n");		
//			printf("\nNandWrite mem_offset = %d, len = %d\n",(int)Memory_Offset,nandlen);
//		Write_LEDn_Status(LED3,1);
			FlashWriteOneSector(Memory_Offset, (u8*)Writebuff, nandlen);
//		Write_LEDn_Status(LED3,0);
			break;
		case MAL_SD:
//			printf("\nSDWrite mem_offset = %d, len = %d\n",(int)Memory_Offset,Transfer_Length);

				SD_WriteBlock(Memory_Offset, Writebuff, Transfer_Length);

			break;
		default:
			return MAL_FAIL;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK; 
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint64_t Memory_Offset, uint32_t *Readbuff, uint32_t nandlen,uint16_t Transfer_Length) 
{
	u8 STA;
	switch (lun)		//这里,根据lun的值确定所要操作的磁盘
	{
		case MAL_SPIFLASH:			//磁盘0为 SPI FLASH盘	 
			STA=0;
//			printf("\nRead SpiFlash，mem = %d, len = %d\n",(int)Memory_Offset,Transfer_Length);
			W25QXX_Read((u8*)Readbuff, Memory_Offset, Transfer_Length);   		  
			break;	  
		case MAL_NANDFLASH:			//磁盘1为SD卡	
			STA=0;
//			printf("\nRead NandFlash，mem = %d, len = %d\n",(int)Memory_Offset,nandlen);
		  FlashReadOneSector(Memory_Offset, (u8*)Readbuff, nandlen);
			break;		
		case MAL_SD:
//			printf("\nRead SD，mem = %d, len = %d\n",(int)Memory_Offset,Transfer_Length);
			SD_ReadBlock(Memory_Offset, Readbuff, Transfer_Length);
			break;
		default:
			break;
	}
	if(STA!=0)return MAL_FAIL;
	return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
	uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
	
    if(lun == MAL_SPIFLASH){
		Mass_Memory_Size[lun]=SPIFLASH_MEM_SIZE;	//前12M字节
		Mass_Block_Size[lun] =SPIFLASH_SECTOR_SIZE;			//设置SPI FLASH的操作扇区大小为512
		Mass_Block_Count[lun]=Mass_Memory_Size[lun]/Mass_Block_Size[lun];
		return MAL_OK;
	}else if(lun == MAL_NANDFLASH){
		Mass_Block_Count[lun] = FLASH_MAX_SECTOR_ADDR/FLASH_SECTOR_SIZE; //NAND_ZONE_SIZE * NAND_BLOCK_SIZE * NAND_MAX_ZONE ;
		Mass_Block_Size[lun]  = FLASH_SECTOR_SIZE;//NAND_PAGE_SIZE;
		Mass_Memory_Size[lun] = (Mass_Block_Count[lun] * Mass_Block_Size[lun]);
		return MAL_OK;
  }else if(lun == MAL_SD){
//		  SD_GetCardInfo(&SDCardInfo);
//      SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
//      DeviceSizeMul = (SDCardInfo.SD_csd.DeviceSizeMul + 2);

//      if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
//      {
//        Mass_Block_Count[lun] = (SDCardInfo.SD_csd.DeviceSize + 1) * 1024;
//      }
//      else
//      {
//        NumberOfBlocks  = ((1 << (SDCardInfo.SD_csd.RdBlockLen)) / 512);
//        Mass_Block_Count[lun] = ((SDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
//      }
      Mass_Block_Size[lun]  = 512;
			Mass_Block_Count[lun] = 0x40000000/512;
//      Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16)); 
//      Status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
//      if ( Status != SD_OK )
//      {
//        return MAL_FAIL;
//      }
//       
//      Status = SD_SetDeviceMode(SD_DMA_MODE);         
//      if ( Status != SD_OK )
//      {
//        return MAL_FAIL;
//      } 
		Mass_Memory_Size[lun] = (Mass_Block_Count[lun] * Mass_Block_Size[lun]);
		return MAL_OK;
	}		
		return MAL_FAIL;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

