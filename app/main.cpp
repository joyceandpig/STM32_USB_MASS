#include "my_platform.h"
#include "api.h"
#include "stm32_eval.h"
#include "stm32_key.h"
#include "app.h"
#include "stm32_fsmc_nand.h"
#include "sdcard.h"

//0x20000为板子内存总大小，更换MCU时需注意
#define CHIP_RAM_START_Addr		0x20000000
#define CHIP_RAM_SIZE					0xC000

#include "diskio.h"
#include "ff.h"

#include "usmart.h"

extern char Image$$RW_IRAM1$$ZI$$Limit[];

void _mem_init(void)
{
	uint32_t malloc_start, malloc_size;
	
	malloc_start = (uint32_t) &Image$$RW_IRAM1$$ZI$$Limit; //取空闲内存最低地址
	
	malloc_start = (malloc_start + 3) & (~0x03);// ALIGN TO word
	
	malloc_size = CHIP_RAM_START_Addr + CHIP_RAM_SIZE - malloc_start;
	
	u_printf(INFO,"Memory Init is Successful. \r\n \t     User Malloc RAM Size[%dBytes] ≈ [%dKBytes].",malloc_size,malloc_size >> 10);

	sys_meminit((void*)malloc_start, malloc_size);
}

extern unsigned long sys_time;

void main_thread(void *pdata)
{
	u16 ret;
	char buf[512];

	RCC_ClocksTypeDef RCC_Clock;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_GetClocksFreq(&RCC_Clock);//获取系统总线时钟
	SysTick_Config(RCC_Clock.SYSCLK_Frequency / (OS_TICK_RATE_HZ *10));
	u_printf(INFO,"System Clock Frequency:SYSCLK[%dMhz],HCLK[%dMhz],PCLK1[%dMhz],PCLK2[%dMhz].",	\
									RCC_Clock.SYSCLK_Frequency/1000000,		\
									RCC_Clock.HCLK_Frequency/1000000,			\
									RCC_Clock.PCLK1_Frequency/1000000,		\
									RCC_Clock.PCLK2_Frequency/1000000
					);
	/*!< At this stage the microcontroller clock setting is already configured, 
	 this is done through SystemInit() function which is called from startup
	 file (startup_stm32f10x_xx.s) before to branch to application main.
	 To reconfigure the default setting of SystemInit() function, refer to
	 system_stm32f10x.c file
 */     

  /* Initialize LEDs, Key Button, LCD and COM port(USART)
                       ******************************************************/	
//  STM_EVAL_COMInit(COM2, &USART_COM2);//初始化串口2	
	STM_EVAL_LED_Init();			//LED初始化
	STM_EVAL_BUZZERInit();		//蜂鸣器初始化

	user_key_init();				//按键初始化
	Timer2_Init();					//Timer2初始化

	OSStatInit(); 					//初始化UCOS状态

	init_work_thread();			//初始化工作线程
	u_printf(INFO,"System successfully launched.\nThe startup time of %d.%d seconds.", os_time_get() / 1000, os_time_get() % 1000);

	TestNandFlashAsMass();
  printf("\nmain thred\n");			
	while (1)
	{					
		uint16 i,j;
		ret = USART_Read_Data(buf);
		
		if(ret>0)
		{			
			BUZZER_Beep(20);
			switch(buf[0])
			{
				case 'f':					
//					u_printf(INFO,"格式化...return:%d",f_mkfs("1:/",1,4096));//MUST Format for New NANDFLASH !!!	
//					u_printf(INFO,"格式化...return:%d",f_mkfs("2:/",1,4096));//MUST Format for New NANDFLASH !!!	
						printf("\nRemapBlockStatus Table:\n");
						for(i = 0; i < FLASH_BAD_BLOCKS_REMAP; i++){
							printf("%d ",(u8)FlashRemapBlockStatus[i]);
						}	
						printf("\n");
					break;
				case 'q':
//					SoftReset();	
						printf("\nBadBlock Table:\n");
					for(j = 0 ; j < 2 ; j++){
						for(i = 0; i < FLASH_BAD_BLOCKS_REMAP; i++){
							printf("%d ",(u8)FlashBadBlockTable[j][i]);
						}	
					}
						printf("\n");
					break;				
				default:
					break;
			}
		}
		Sleep(20);
	}
}
				
#define DEVELOP_INFO	\
	"* System Information !!!	\r\n \r\n"\
	"* Company Name:成都淞幸科技有限责任公司                                	 	\r\n\r\n"	\
	"* Address:四川省成都市大件路白家段280号空港国际城7栋1单元11层 							\r\n\r\n"	\
	"* WebSite:www.sunsheen.cn     E-mail:feng.guangrong@sunsheen.cn          \r\n\r\n"	\
	"* Fax:028-8580 5757           Tel:028-6445 2893                         	\r\n\r\n"	\
	"* Project Name:DZ_HomeBasic Project                                     	\r\n\r\n"	\
	"* Build time:("__DATE__ " - " __TIME__ ")                               	\r\n\r\n"	\
	"* SoftWare Revision:"VERSION"         Author:FengGuangrong       				\r\n\r\n"	\
	"* (C) CopyRight 2016-2026 SUNSHEEN Technology. 													\r\n\r\n"	\
	"\r\n"/*开发信息描述*/	

/**
  *   Function name : Fill_Buffer
  * @brief  Fill the buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  */
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp+ Offset;
  }
}



void Soft_Info(void)
{
	CLC_CMD;TELNET_COLOR(CONTROL_BLACK,CONTROL_WHITE);/*设置终端显示颜色*/
	
	MyPrintf("==============================================================================\r\n");
	
	MyPrintf("%s",DEVELOP_INFO);/*打印开发信息*/
	
	MyPrintf("* __STM32F10X_STDPERIPH_VERSION %d.%d.%d \r\n\r\n",\
					__STM32F10X_STDPERIPH_VERSION_MAIN,\
					__STM32F10X_STDPERIPH_VERSION_SUB1,\
					__STM32F10X_STDPERIPH_VERSION_SUB2\
				);/*打印库函数版本*/
	
#ifdef RELEASE_VERSION	
	MyPrintf("* 96bit device ID[%04X-%04X-%08X-%08X] \r\n\r\n",\
					*(__IO uint16_t*)(DEVICE_ID_BASE_Add + 0x00),\
					*(__IO uint16_t*)(DEVICE_ID_BASE_Add + 0x02),\
					*(__IO uint32_t*)(DEVICE_ID_BASE_Add + 0x04),\
					*(__IO uint32_t*)(DEVICE_ID_BASE_Add + 0x08)\
				);/*打印产品唯一身份标识寄存器(96位)*/	
#endif
	
#ifdef RELEASE_VERSION		
	MyPrintf("* Internal Flash：%dK Byte！  \r\n\r\n",*(__IO uint16_t*)(0x1FFFF7E0));/*打印产品内部flash大小*/	
#endif

#ifdef NOR_FLASH
  /*读取Nor Flash ID并打印*/
  FSMC_NOR_ReadID(&NOR_ID);
	
  MyPrintf("* Ext. NOR Flash：%dK Byte！ ",(1<<(FSMC_NOR_Read_Device_Size()-10)));/*打印NOR Flash大小*/

	MyPrintf("\tID[%04X-%4X]	\r\n\r\n",NOR_ID.Manufacturer_Code,NOR_ID.Device_Code1);
#endif

#if 0//def NAND_FLASH	
  /*读取Nand Flash ID并打印*/
  FSMC_NAND_ReadID(&NAND_ID);
  printf("\n\r Nand Flash ID:0x%x\t 0x%x\t 0x%x\t 0x%x",NAND_ID.Maker_ID,NAND_ID.Device_ID,
  	                                                    NAND_ID.Third_ID,NAND_ID.Fourth_ID);
	/*擦除待写入数据的块*/
    status = FSMC_NAND_EraseBlock(0);

    /*将写Nand Flash的数据BUFFER填充为从0x25开始的连续递增的一串数据 */
    Fill_Buffer(TxBuffer, BUFFER_SIZE , 0);
//    /*将数据写入到Nand Flash中。WriteReadAddr:读写的起始地址*/	
    status = FSMC_NAND_WriteSmallPage(TxBuffer, 0, PageNumber);
printf("\r\n================%d\r\n\r\n",status);
    /*从Nand Flash中读回刚写入的数据。riteReadAddr:读写的起始地址*/
    status = FSMC_NAND_ReadSmallPage (RxBuffer, 0, PageNumber);
printf("\r\n================%d\r\n\r\n",status);
		printf("\r\n================================================================\r\n\r\n");
		for(j = 0; j < BUFFER_SIZE; j++)
    {
			if(j%16 == 0)
				printf("\r\n");	

			if(j%512 == 0)
			{
				i++;
				printf("\r\n\r\n\r\n\r\n");
			}
			
			printf("%4u ,",RxBuffer[j]);
    }
#endif	
	
	MyPrintf("* ARMCC VERSION: %d.%02d(Build.%d) \r\n\r\n",__ARMCC_VERSION/1000000,(__ARMCC_VERSION%1000000)/10000,__ARMCC_VERSION%10000);/*打印编译器版本*/	

	MyPrintf("* Optimization = Level%d(-O%d) \r\n\r\n",__OPTIMISE_LEVEL,__OPTIMISE_LEVEL);/*打印优化等级*/	 
	
	MyPrintf("==============================================================================\r\n");
	
	MyPrintf("\r\n");
	/*打印软件信息*/
}

int main(void)
{
//	u16 Sta = 0;
//	SD_CardInfo SDInfo;
#ifdef NOR_FLASH
  FSMC_NOR_Init();/*配置与SRAM连接的FSMC BANK1 NOR/SRAM2*/
#endif
	
#ifdef EXT_SRAM  
  FSMC_SRAM_Init();/*配置与SRAM连接的FSMC BANK1 NOR/SRAM3*/
#endif
	
	
  STM_EVAL_COMInit(COM1, &USART_COM1);//初始化串口1	
	usmart_dev.init(72);	//初始化USMART	
	
	Soft_Info();//软件信息
	
	
/* Initialize Operating System,uC/OS-II or uC/OS-III
                       ******************************************************/	
#ifdef OS_UCOS
#ifdef UCOS_V3
	OS_ERR err;
	OSInit(&err);
	u_printf(INFO,"Initialize Operating System.	\n uC/OS-III.");
#else
	OSInit();
	u_printf(INFO,"Initialize Operating System.	\n uC/OS-II.");
#endif
#endif
	_mem_init(); //初始化内存分配	
	msg_q_init();//
	u_printf(INFO,"Message Queue Init is Successful.");
	
	thread_create(main_thread, 0, TASK_MAIN_PRIO, 0, TASK_MAIN_STACK_SIZE, "main_thread");
	
#ifdef OS_UCOS
#ifdef UCOS_V3

	OSStart(&err);
	u_printf(INFO,"Start uC/OS-III...");
#else
	OSStart();
	u_printf(INFO,"Start uC/OS-II...");
#endif
#endif
	return 0;
}
