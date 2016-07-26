/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "my_platform.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
//#ifdef	SPI_FLASH
//	#include "w25qxx.h" 
//	#define FLASH_SECTOR_SIZE 	512			  
//	//¶ÔÓÚW25Q16
//	//2M×Ö½Ú¸øfatfs	 			    
//	u16	    FLASH_SECTOR_COUNT=2048*2;	//W25Q16,¹²2M×Ö½Ú¸øFATFSÕ¼ÓÃ
//	#define FLASH_BLOCK_SIZE   	8     	//Ã¿¸öBLOCKÓÐ8¸öÉÈÇø
//#endif

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

//DSTATUS disk_initialize (
//	BYTE drv				/* Physical drive nmuber (0..) */
//)
//{
////	DSTATUS stat;
//	int result;
//#ifdef	SPI_FLASH	
//	u16 id;
//#endif
//	
//	switch (drv) {
//#ifdef	SPI_FLASH		
//	case SPI_FLASH :
//	id = W25QXX_Init();//W25QXX³õÊ¼»¯
//	if(id>= W25Q80 && id<=W25Q128)
//	{
//		static const char * Chip_Table[] =  \
//		{
//			"W25Q80[0xEF13]",\
//			"W25Q16[0xEF14]",\
//			"W25Q32[0xEF15]",\
//			"W25Q64[0xEF16]",\
//			"W25Q128[0xEF17]"\
//		};

//		u_printf(INFO,"%s ³õÊ¼»¯³É¹¦£¡",Chip_Table[id - 0XEF13]);	
//		
//		if(W25QXX_TYPE != id)
//		{
//			u_printf(WARN,"µ±Ç°Ð¾Æ¬ÓëÄ¬ÈÏÐ¾Æ¬%s²»Æ¥Åä",Chip_Table[W25QXX_TYPE - 0XEF13]);
//		}
//		return 0;
//	}
//	else
//	{
//		u_printf(INFO,"SPI FLASH ³õÊ¼»¯Ê§°Ü£¡id= %x",id);
//		break;	
//	}
//#endif
//	
//#ifdef	FSMC_NAND	
//	case FSMC_NAND:	
////		FSMC_NAND_Init();
////		FSMC_NAND_ReadID(&NAND_ID);	
//		u_printf(INFO,"NAND Flash ³õÊ¼»¯³É¹¦£¡");
//		return 0;
//#endif
//	
//	default:
//		result = result;
//		break;
//	}		
//	return STA_NOINIT;
//}



///*-----------------------------------------------------------------------*/
///* Return Disk Status                                                    */

//DSTATUS disk_status (
//	BYTE drv		/* Physical drive nmuber (0..) */
//)
//{
////	DSTATUS stat;
////	int result;

//	switch (drv) {
//#ifdef	SPI_FLASH			
//		case SPI_FLASH :

//		return 0;
//#endif
//		
//#ifdef	FSMC_NAND		
//	case FSMC_NAND:
//		return 0;
//#endif
//	default:
//		break;
//	
//	}
//	return STA_NOINIT;
//}



///*-----------------------------------------------------------------------*/
///* Read Sector(s)                                                        */

//DRESULT disk_read (
//	BYTE drv,		/* Physical drive nmuber (0..) */
//	BYTE *buff,		/* Data buffer to store read data */
//	DWORD sector,	/* Sector address (LBA) */
//	BYTE count		/* Number of sectors to read (1..255) */
//)
//{
////	DRESULT res;
//	int result;
//	
//	if (!count){  return RES_PARERR;}	
//	
//	switch (drv) {
//		
//#ifdef	SPI_FLASH					
//		case SPI_FLASH :
//		for(;count>0;count--)
//		{
//			W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
//			sector++;
//			buff+=FLASH_SECTOR_SIZE;
//		}
//		return RES_OK;
//#endif
//		
//#ifdef	FSMC_NAND					
//	case FSMC_NAND:
////		result = FSMC_NAND_ReadSmallPage(buff, sector, count);                                                
//    if(result & NAND_READY)		 
//			return RES_OK; 
//    else 
//			return RES_ERROR;			
//#endif
//	default:
//		result = result;
//		break;		
//	}
//	return RES_PARERR;
//}



///*-----------------------------------------------------------------------*/
///* Write Sector(s)                                                       */

//#if _READONLY == 0
//DRESULT disk_write (
//	BYTE drv,			/* Physical drive nmuber (0..) */
//	const BYTE *buff,	/* Data to be written */
//	DWORD sector,		/* Sector address (LBA) */
//	BYTE count			/* Number of sectors to write (1..255) */
//)
//{
////	DRESULT res;
//	int result;

//#ifdef	FSMC_NAND	
//	uint32_t BackupBlockAddr;
//	uint32_t WriteBlockAddr;
//	uint16_t IndexTmp = 0;
//	uint16_t OffsetPage;
//#endif
//	
//  if (!count){  return RES_PARERR;}	
//	
//	switch (drv) {
//#ifdef	SPI_FLASH			
//		case SPI_FLASH :
//			for(;count>0;count--)
//			{										    
//				W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
//				sector++;
//				buff+=FLASH_SECTOR_SIZE;
//			}
//			return RES_OK;
//#endif
//			
//#ifdef	FSMC_NAND	
//	case FSMC_NAND:
//			/* NAND memory write page at block address*/
//			WriteBlockAddr = (sector/NAND_BLOCK_SIZE);
//			/* NAND memory backup block address*/
//			BackupBlockAddr = (WriteBlockAddr + (NAND_MAX_ZONE/2)*NAND_ZONE_SIZE);
//			OffsetPage = sector%NAND_BLOCK_SIZE;
//    
//   /* Erase the NAND backup Block */
//      result = FSMC_NAND_EraseBlock(BackupBlockAddr*NAND_BLOCK_SIZE);
//  
//      /* Backup the NAND Write Block to High zone*/
//  
//			 for (IndexTmp = 0; IndexTmp < NAND_BLOCK_SIZE; IndexTmp++ )
//			 {
//				FSMC_NAND_MoveSmallPage (WriteBlockAddr*NAND_BLOCK_SIZE+IndexTmp,BackupBlockAddr*NAND_BLOCK_SIZE+IndexTmp);
//			 }
//			
//			 /* Erase the NAND Write Block */
//			 result = FSMC_NAND_EraseBlock(WriteBlockAddr*NAND_BLOCK_SIZE);
//			
//					/*return write the block  with modify*/
//			 for (IndexTmp = 0; IndexTmp < NAND_BLOCK_SIZE; IndexTmp++ )
//			 {
//					if((IndexTmp>=OffsetPage)&&(IndexTmp < (OffsetPage+count)))
//					{
//						 FSMC_NAND_WriteSmallPage((uint8_t *)buff, WriteBlockAddr*NAND_BLOCK_SIZE+IndexTmp, 1);
//						 buff = (uint8_t *)buff + NAND_PAGE_SIZE;
//					}
//					else
//					{
//						 FSMC_NAND_MoveSmallPage (BackupBlockAddr*NAND_BLOCK_SIZE+IndexTmp,WriteBlockAddr*NAND_BLOCK_SIZE+IndexTmp);
//					}
//			 }    
//			 
//				if(result == NAND_READY)
//					return RES_OK;
//				else
//					return RES_ERROR;
//#endif
//	default:
//		result = result;
//		break;		
//	}
//	return RES_PARERR;
//}
//#endif /* _READONLY */



///*-----------------------------------------------------------------------*/
///* Miscellaneous Functions                                               */

//DRESULT disk_ioctl (
//	BYTE drv,		/* Physical drive nmuber (0..) */
//	BYTE ctrl,		/* Control code */
//	void *buff		/* Buffer to send/receive control data */
//)
//{
//	DRESULT res;
////	int result;
//	
//	res = RES_OK;
//		
//	switch (drv) {
//#ifdef	SPI_FLASH			
//		case SPI_FLASH:
//			
//	    switch(ctrl)
//	    {
//		    case CTRL_SYNC:
//						res = RES_OK; 
//		        break;	 
//		    case GET_SECTOR_SIZE:
//		        *(WORD*)buff = FLASH_SECTOR_SIZE;
//		        res = RES_OK;
//		        break;	 
//		    case GET_BLOCK_SIZE:
//		        *(WORD*)buff = FLASH_BLOCK_SIZE;
//		        res = RES_OK;
//		        break;	 
//		    case GET_SECTOR_COUNT:
//		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
//		        res = RES_OK;
//		        break;
//		    default:
//		        res = RES_PARERR;
//		        break;
//	    }
//			return res;
//#endif
//			
//#ifdef	FSMC_NAND	
//		case FSMC_NAND:    
//      switch(ctrl)
//      {
//       case CTRL_SYNC:
//           break;
//				case GET_BLOCK_SIZE:
//           *(DWORD*)buff = NAND_BLOCK_SIZE;
//           break;
//				case GET_SECTOR_COUNT:
//           *(DWORD*)buff = (((NAND_MAX_ZONE/2) * NAND_ZONE_SIZE) * NAND_BLOCK_SIZE);
//           break;
//				case GET_SECTOR_SIZE:
//           *(WORD*)buff = NAND_PAGE_SIZE;
//           break;
//       default:
//           res = RES_PARERR;
//           break;
//			}
//      return res;	
//#endif
//			 default:
//				 res = res;
//				 break;
//	}
//	return RES_PARERR;
//}
//DWORD get_fattime (void)
//{ 
//	return 0;
//}


#include "ff.h"			/* Declarations of FatFs API */

static const char * FR_Table[]= 
{
    "FR_OK£º³É¹¦",                                      /* (0) Succeeded */
    "FR_DISK_ERR£ºµ×²ãÓ²¼þ´íÎó",                      /* (1) A hard error occurred in the low level disk I/O layer */
    "FR_INT_ERR£º¶ÏÑÔÊ§°Ü",                              /* (2) Assertion failed */
    "FR_NOT_READY£ºÎïÀíÇý¶¯Ã»ÓÐ¹¤×÷",                  /* (3) The physical drive cannot work */
    "FR_NO_FILE£ºÎÄ¼þ²»´æÔÚ",                          /* (4) Could not find the file */
    "FR_NO_PATH£ºÂ·¾¶²»´æÔÚ",                          /* (5) Could not find the path */
    "FR_INVALID_NAME£ºÎÞÐ§ÎÄ¼þÃû",                      /* (6) The path name format is invalid */
    "FR_DENIED£ºÓÉÓÚ½ûÖ¹·ÃÎÊ»òÕßÄ¿Â¼ÒÑÂú·ÃÎÊ±»¾Ü¾ø",  /* (7) Access denied due to prohibited access or directory full */
    "FR_EXIST£ºÓÉÓÚ·ÃÎÊ±»½ûÖ¹·ÃÎÊ±»¾Ü¾ø",              /* (8) Access denied due to prohibited access */
    "FR_INVALID_OBJECT£ºÎÄ¼þ»òÕßÄ¿Â¼¶ÔÏóÎÞÐ§",          /* (9) The file/directory object is invalid */
    "FR_WRITE_PROTECTED£ºÎïÀíÇý¶¯±»Ð´±£»¤",              /* (10) The physical drive is write protected */
    "FR_INVALID_DRIVE£ºÂß¼­Çý¶¯ºÅÎÞÐ§",                  /* (11) The logical drive number is invalid */
    "FR_NOT_ENABLED£º¾íÖÐÎÞ¹¤×÷Çø",                      /* (12) The volume has no work area */
    "FR_NO_FILESYSTEM£ºÃ»ÓÐÓÐÐ§µÄFAT¾í",              /* (13) There is no valid FAT volume */
    "FR_MKFS_ABORTED£ºÓÉÓÚ²ÎÊý´íÎóf_mkfs()±»ÖÕÖ¹",             /* (14) The f_mkfs() aborted due to any parameter error */
    "FR_TIMEOUT£ºÔÚ¹æ¶¨µÄÊ±¼äÄÚÎÞ·¨»ñµÃ·ÃÎÊ¾íµÄÐí¿É",         /* (15) Could not get a grant to access the volume within defined period */
    "FR_LOCKED£ºÓÉÓÚÎÄ¼þ¹²Ïí²ßÂÔ²Ù×÷±»¾Ü¾ø",                 /* (16) The operation is rejected according to the file sharing policy */
    "FR_NOT_ENOUGH_CORE£ºÎÞ·¨·ÖÅä³¤ÎÄ¼þÃû¹¤×÷Çø",             /* (17) LFN working buffer could not be allocated */
    "FR_TOO_MANY_OPEN_FILES£ºµ±Ç°´ò¿ªµÄÎÄ¼þÊý´óÓÚ_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
    "FR_INVALID_PARAMETER£º²ÎÊýÎÞÐ§"                         /* (19) Given parameter is invalid */
};

#define BUFFER_SIZE         2048
uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];

#define Fatfs_assert_param(y,x)	do{WARN_PRINT_TIME();	\
															MyPrintf("Warning! (%d):\t",__LINE__); \
															MyPrintf("%s %s \r\n",y,FR_Table[x]);}while(0)

///////////////////////////////¹«¹²ÎÄ¼þÇø,Ê¹ÓÃmallocµÄÊ±ºò////////////////////////////////////////////
FATFS fs,fs1;//Âß¼­´ÅÅÌ¹¤×÷Çø.	 
FIL file;	  		//ÎÄ¼þ1
FIL ftemp;	  		//ÎÄ¼þ2.
UINT br,bw;			//¶ÁÐ´±äÁ¿
FILINFO fileinfo;	//ÎÄ¼þÐÅÏ¢
DIR dir;  			//Ä¿Â¼

u8 *fatbuf = (void *)0;			//SD¿¨Êý¾Ý»º´æÇø
extern u8 Max_Lun;		
extern vu8 USB_STATUS_REG;

#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h" 
#include "stm32_fsmc_nand.h"															
															
/*******************************************************************************
* Function Name  : 
* Description    :
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void USB_CONNECT_INIT(void){
		USB_Port_Set(0); 	//USB?è???a
		USB_Port_Set(1);	//USB?ù'?á??ó 
		//USB????
		USB_Interrupts_Config();    
		Set_USBClock();
		USB_Init();	 
}
/*******************************************************************************
* Function Name  : 
* Description    :
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void MAL_Disk_Init(void){
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		//Init spi 
		MAL_Init(MAL_SPIFLASH);
		//Init  nand
		MAL_Init(MAL_NANDFLASH);
		//Init sd
//		MAL_Init(MAL_SD);
}
/*******************************************************************************
* Function Name  : 
* Description         :
* Input                  : 
* Output               : 
* Return               : 
*******************************************************************************/                                            												
void TestNandFlashAsMass(void)
{
		u8 offline_cnt=0;
		u8 tct=0;
		u8 USB_STA;
		u8 Divece_STA;	
	
		MAL_Disk_Init();//´ÅÅÌ³õÊ¼»¯
		USB_CONNECT_INIT();//usbÆô¶¯Á¬½Ó
	  while (bDeviceState != CONFIGURED);	  //µÈ´ýUSBÃ¶¾Ù³É¹¦
	  u_printf(DBG,"USB Enum Success");	
	
		while(1)
		{
			Sleep(1);				  
			if(USB_STA!=USB_STATUS_REG)//×´Ì¬¸Ä±äÁË 
			{	 						   
				if(USB_STATUS_REG&0x01)//ÕýÔÚÐ´		  
				{
					u_printf(DBG,"USB Writing...");//ÌáÊ¾USBÕýÔÚÐ´ÈëÊý¾Ý	 
				}
				if(USB_STATUS_REG&0x02)//ÕýÔÚ¶Á
				{
					u_printf(DBG,"USB Reading...");//ÌáÊ¾USBÕýÔÚ¶Á³öÊý¾Ý  		 
				}	 										  
				if(USB_STATUS_REG&0x04)u_printf(DBG,"USB Write Err ");//ÌáÊ¾Ð´Èë´íÎó
 
				if(USB_STATUS_REG&0x08)u_printf(DBG,"USB Read  Err ");//ÌáÊ¾¶Á³ö´íÎó
 
				USB_STA=USB_STATUS_REG;//¼ÇÂ¼×îºóµÄ×´Ì¬
			}
			if(Divece_STA!=bDeviceState) 
			{
				if(bDeviceState==CONFIGURED)u_printf(DBG,"USB Connected    ");//ÌáÊ¾USBÁ¬½ÓÒÑ¾­½¨Á¢
				Divece_STA=bDeviceState;
//				return ;
			}
			tct++;
			if(tct==200)
			{
				tct=0;
				if(USB_STATUS_REG&0x10)
				{
					offline_cnt=0;//USBÁ¬½ÓÁË,ÔòÇå³ýoffline¼ÆÊýÆ÷
					bDeviceState=CONFIGURED;
				}else//Ã»ÓÐµÃµ½ÂÖÑ¯ 
				{
					offline_cnt++;  
					if(offline_cnt>10)bDeviceState=UNCONNECTED;//2sÄÚÃ»ÊÕµ½ÔÚÏß±ê¼Ç,´ú±íUSB±»°Î³öÁË
				}
				USB_STATUS_REG=0;
			}		
		}
	
}	
