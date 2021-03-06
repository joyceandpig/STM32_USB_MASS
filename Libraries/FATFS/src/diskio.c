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


#include "ff.h"			/* Declarations of FatFs API */

static const char * FR_Table[]= 
{
    "FR_OK：成功",                                      /* (0) Succeeded */
    "FR_DISK_ERR：底层硬件错误",                      /* (1) A hard error occurred in the low level disk I/O layer */
    "FR_INT_ERR：断言失败",                              /* (2) Assertion failed */
    "FR_NOT_READY：物理驱动没有工作",                  /* (3) The physical drive cannot work */
    "FR_NO_FILE：文件不存在",                          /* (4) Could not find the file */
    "FR_NO_PATH：路径不存在",                          /* (5) Could not find the path */
    "FR_INVALID_NAME：无效文件名",                      /* (6) The path name format is invalid */
    "FR_DENIED：由于禁止访问或者目录已满访问被拒绝",  /* (7) Access denied due to prohibited access or directory full */
    "FR_EXIST：由于访问被禁止访问被拒绝",              /* (8) Access denied due to prohibited access */
    "FR_INVALID_OBJECT：文件或者目录对象无效",          /* (9) The file/directory object is invalid */
    "FR_WRITE_PROTECTED：物理驱动被写保护",              /* (10) The physical drive is write protected */
    "FR_INVALID_DRIVE：逻辑驱动号无效",                  /* (11) The logical drive number is invalid */
    "FR_NOT_ENABLED：卷中无工作区",                      /* (12) The volume has no work area */
    "FR_NO_FILESYSTEM：没有有效的FAT卷",              /* (13) There is no valid FAT volume */
    "FR_MKFS_ABORTED：由于参数错误f_mkfs()被终止",             /* (14) The f_mkfs() aborted due to any parameter error */
    "FR_TIMEOUT：在规定的时间内无法获得访问卷的许可",         /* (15) Could not get a grant to access the volume within defined period */
    "FR_LOCKED：由于文件共享策略操作被拒绝",                 /* (16) The operation is rejected according to the file sharing policy */
    "FR_NOT_ENOUGH_CORE：无法分配长文件名工作区",             /* (17) LFN working buffer could not be allocated */
    "FR_TOO_MANY_OPEN_FILES：当前打开的文件数大于_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
    "FR_INVALID_PARAMETER：参数无效"                         /* (19) Given parameter is invalid */
};

#define BUFFER_SIZE         2048
uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];

#define Fatfs_assert_param(y,x)	do{WARN_PRINT_TIME();	\
															MyPrintf("Warning! (%d):\t",__LINE__); \
															MyPrintf("%s %s \r\n",y,FR_Table[x]);}while(0)

///////////////////////////////公共文件区,使用malloc的时候////////////////////////////////////////////
FATFS fs,fs1;//逻辑磁盘工作区.	 
FIL file;	  		//文件1
FIL ftemp;	  		//文件2.
UINT br,bw;			//读写变量
FILINFO fileinfo;	//文件信息
DIR dir;  			//目录

u8 *fatbuf = (void *)0;			//SD卡数据缓存区
															
															
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
		USB_Port_Set(0); 	//USB?�???a
		USB_Port_Set(1);	//USB?�'?�??� 
		//USB????
		USB_Interrupts_Config();    
		Set_USBClock();
		USB_Init();	 
	
		while (bDeviceState != CONFIGURED);	  //等待USB枚举成功
	  u_printf(DBG,"USB Enum Success");	
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
		MAL_Init(MAL_SD);
}
void fatfstest(void)
{
	u8 res=0;	
	DWORD clust;
	unsigned int r;
	FATFS *pfs;
	#if 0
		const TCHAR *Ptr = "1:/";
		const TCHAR *fl_Path = "1:/test.dat";
	#else
		const TCHAR *Ptr = "3:/";
		const TCHAR *fl_Path = "3:/NandTest.doc";
	#endif

	
	res = f_mount(&fs,Ptr,1); 				//
	if (res != FR_OK){
		Fatfs_assert_param("挂载失败，尝试格式化...",res);	
//		res=f_mkfs(Ptr,1,4096);  //MUST Format for New NANDFLASH !!!
		Fatfs_assert_param("格式化...",res);	
	}else{
		Fatfs_assert_param("挂载成功...",res);
	}
	
	res = f_getfree(Ptr, &clust, &pfs);//??????,????0;"/"??"0:/"
	Fatfs_assert_param("获取磁盘根目录",res);

	
 //for write
  res=f_open(&file,fl_Path,FA_OPEN_EXISTING |FA_READ);
	if(res != FR_OK){
		Fatfs_assert_param("open  /FlashTest.txt 文件打开错误",res);
		res=f_open(&file,fl_Path,FA_OPEN_ALWAYS |FA_WRITE);
		Fatfs_assert_param("/FlashTest.txt 重新创建!",res);
		
		sprintf((char *)TxBuffer,"现在文件系统!");		
		res=f_write(&file,TxBuffer,512,&r);
		Fatfs_assert_param("write  /FlashTest.txt",res);
		f_close(&file);	//??	
		u_printf(INFO,"文件保存!");
		
		//????
		res=f_open(&file,fl_Path,FA_OPEN_EXISTING | FA_READ );
		Fatfs_assert_param("open  /FlashTest.txt",res);	
		if(res != FR_OK){
			u_printf(ERR,"文件打开错误!");
			while(1);
		}		
		Sleep(200);
	}else{
		Fatfs_assert_param("open  /FlashTest.txt 打开成功!",res);
	}
	
	// for read	
	res=f_read(&file,RxBuffer,512,&r);
	Fatfs_assert_param("read  /FlashTest.txt",res);
	Sleep(200);
	if(res || r == 0)	u_printf(TRACK);

	u_printf(DBG,"[%s] Recv: \n\t%s",Ptr,RxBuffer);
	u_printf(TRACK);       
	f_close(&file);
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
	
//		fatfstest();
		MAL_Disk_Init();//磁盘初始化
//		W25QXX_Erase_Chip();
		
		USB_CONNECT_INIT();//usb启动连接


		while(1)
		{
			Sleep(1);				  
			if(USB_STA!=USB_STATUS_REG)//状态改变了 
			{	 						   
				if(USB_STATUS_REG&0x01)//正在写		  
				{
					u_printf(DBG,"USB Writing...");//提示USB正在写入数据	 
				}
				if(USB_STATUS_REG&0x02)//正在读
				{
					u_printf(DBG,"USB Reading...");//提示USB正在读出数据  		 
				}	 										  
				if(USB_STATUS_REG&0x04)u_printf(DBG,"USB Write Err ");//提示写入错误
 
				if(USB_STATUS_REG&0x08)u_printf(DBG,"USB Read  Err ");//提示读出错误
 
				USB_STA=USB_STATUS_REG;//记录最后的状态
			}
			if(Divece_STA!=bDeviceState) 
			{
				if(bDeviceState==CONFIGURED)u_printf(DBG,"USB Connected    ");//提示USB连接已经建立
				Divece_STA=bDeviceState;
			}
			tct++;
			if(tct==200)
			{
				tct=0;
				if(USB_STATUS_REG&0x10)
				{
					offline_cnt=0;//USB连接了,则清除offline计数器
					bDeviceState=CONFIGURED;
				}else//没有得到轮询 
				{
					offline_cnt++;  
					if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s内没收到在线标记,代表USB被拔出了
				}
				USB_STATUS_REG=0;
			}		
		}
	
}	
