/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "my_platform.h"
#include "w25qxx.h"
#include "stm32_fsmc_nand.h"
#include "sdcard.h"
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
extern SD_CardInfo SDCardInfo;
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
//	DSTATUS stat;
	int result;
#ifdef	SPI_FLASH	
	u16 id;
#endif
#ifdef FSMC_NAND
	u8 i,NandIDBuf[5];
#endif

	
	switch (drv) {
#ifdef	SPI_FLASH		
	case SPI_FLASH :
	id = W25QXX_Init();//W25QXX初始化
	if(id>= W25Q80 && id<=W25Q128)
	{
		static const char * Chip_Table[] =  \
		{
			"W25Q80[0xEF13]",\
			"W25Q16[0xEF14]",\
			"W25Q32[0xEF15]",\
			"W25Q64[0xEF16]",\
			"W25Q128[0xEF17]"\
		};

		u_printf(INFO,"%s 初始化成功！",Chip_Table[id - 0XEF13]);	
		
		if(W25QXX_TYPE != id)
		{
			u_printf(WARN,"当前芯片与默认芯片%s不匹配",Chip_Table[W25QXX_TYPE - 0XEF13]);
		}
		return 0;
	}
	else
	{
		u_printf(INFO,"SPI FLASH 初始化失败！id= %x",id);
		break;	
	}
#endif
	
#ifdef	FSMC_NAND	
	case FSMC_NAND:	
		FlashInit();
		FlashReadId(NandIDBuf);
		u_printf(INFO,"NAND Flash 初始化成功,ID =");
		u_printf(INFO," %x",NandIDBuf[0] << 24 |
												NandIDBuf[1] << 16 |
												NandIDBuf[2] << 8 |
												NandIDBuf[3]);
		return 0;
#endif
		
#ifdef SDIO_SD_CARD
	case SDIO_SD_CARD:
				result = SD_Init();  //SD卡接口初始化
				result = SD_GetCardInfo(&SDCardInfo); //获取SD卡信息
				result = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
				result = SD_EnableWideBusOperation(SDIO_BusWide_1b);//设置SDIO接口数据宽度
				result = SD_SetDeviceMode(SD_DMA_MODE);//设置工作模式
		return 0;
#endif
	
	default:
		result = result;
		break;
	}		
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	u8 result;

	switch (drv) {
#ifdef	SPI_FLASH			
	case SPI_FLASH:
		stat = W25QXX_ReadSR();
//		stat ? u_printf(INFO,"\nSPIFlash Busy\n"):u_printf(INFO,"\nSPIFlash Free\n");
		return 0;
#endif
		
#ifdef	FSMC_NAND		
	case FSMC_NAND:
		stat = FlashReadStatus();
//		stat ? u_printf(INFO,"\nNandFlash Busy\n"):u_printf(INFO,"\nNandFlash Free\n");
		return 0;
#endif
#ifdef SDIO_SD_CARD
	case SDIO_SD_CARD:
		stat = SD_GetTransferState();
		return 0;
#endif
	
	default:
		break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	DRESULT res;
	int result;
	if (!count){  return RES_PARERR;}	
	
	switch (drv) {
		
#ifdef	SPI_FLASH					
		case SPI_FLASH :
		for(;count>0;count--)
		{
			W25QXX_Read(buff,sector*SPIFLASH_SECTOR_SIZE,SPIFLASH_SECTOR_SIZE);
			sector++;
			buff+=SPIFLASH_SECTOR_SIZE;
		}
		return RES_OK;
#endif
		
#ifdef	FSMC_NAND					
	case FSMC_NAND:
		(uint32)result =FlashReadOneSector((uint32)(sector * FLASH_SECTOR_SIZE), (u8 *)buff, count-1);  

    if(!result)		 
			return RES_OK; 
    else 
			return RES_ERROR;			
#endif
		
#ifdef SDIO_SD_CARD
	case SDIO_SD_CARD:
		res =SD_ReadBlock(sector, (u32 *)buff, count);
    if(!res)		 
			return RES_OK; 
    else 
			return RES_ERROR;	
#endif
	default:
		result = result;
		break;		
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	DRESULT res;
	int result;

#ifdef	FSMC_NAND	
	uint32_t BackupBlockAddr;
	uint32_t WriteBlockAddr;
	uint16_t IndexTmp = 0;
	uint16_t OffsetPage;
#endif
	
  if (!count){  return RES_PARERR;}	
	
	switch (drv) {
#ifdef	SPI_FLASH			
		case SPI_FLASH :
			for(;count>0;count--)
			{										    
				W25QXX_Write((u8*)buff,sector*SPIFLASH_SECTOR_SIZE,SPIFLASH_SECTOR_SIZE);
				sector++;
				buff+=SPIFLASH_SECTOR_SIZE;
			}
			return RES_OK;
#endif
			
#ifdef	FSMC_NAND	
	case FSMC_NAND:
			(uint32)result = FlashWriteOneSector((uint32)(sector * FLASH_SECTOR_SIZE),(u8 *)buff, count-1);
			if(result == 0)
				return RES_OK;
			else
				return RES_ERROR;
#endif

#ifdef SDIO_SD_CARD
	case SDIO_SD_CARD:
		res =SD_WriteBlock(sector, (u32 *)buff, count);
    if(!res)		 
			return RES_OK; 
    else 
			return RES_ERROR;
#endif
	default:
		result = result;
		break;		
	}
	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;
	
	res = RES_OK;
		
	switch (drv) {
#ifdef	SPI_FLASH			
		case SPI_FLASH:
			
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
						res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = SPIFLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = SPIFLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SPIFLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
			return res;
#endif
			
#ifdef	FSMC_NAND	
		case FSMC_NAND:    
      switch(ctrl)
      {
       case CTRL_SYNC:
           break;
				case GET_BLOCK_SIZE:
           *(DWORD*)buff = FLASH_BLOCK_SIZE;
           break;
				case GET_SECTOR_COUNT:
           *(DWORD*)buff = (FLASH_MAX_ADDR) / NAND_BLOCK_SIZE;
           break;
				case GET_SECTOR_SIZE:
           *(WORD*)buff = FLASH_SECTOR_SIZE;
           break;
       default:
           res = RES_PARERR;
           break;
			}
      return res;	
#endif
			
#ifdef SDIO_SD_CARD
			switch(ctrl)
	    {
		    case CTRL_SYNC:
						res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = SD_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = SD_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SD_MEMORY_SIZE/SD_SECTOR_SIZE;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
			return res;
#endif
			 default:
				 res = res;
				 break;
	}
	return RES_PARERR;
}
DWORD get_fattime (void)
{ 
	return 0;
}

