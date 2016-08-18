#include "my_platform.h"

int dbg_level = 1;

void delay_1us(void)
{
	int i = 25;
	while (i--)
		;
}

void delay_us(uint32_t us)
{
	while (us--)
		delay_1us();
}

void delay_ms(uint32_t ms)
{
	while (ms--)
		delay_us(1000);
}

void dev_monitor_task(void *arg)
{
	sleep(2000);
	while (1)
	{
		sleep(1000);
	}
}

/**
 *读一个随机数
 */
uint32_t get_random(void)
{
	return os_time_get();
}

void soft_reset(void)
{
	u_printf(INFO,"system will reset \r\n");
	NVIC_SystemReset();
  	__DSB();  
	while (1);
}

void HardFault_Handler()
{
	u8 i = 5;
#ifdef	OS_UCOS
	OSIntEnter();  
#endif  


	u_printf(ERR_FUN,"");
	Telnet_Control("System will reset....%d",i);
	while (1)
	{
		Sleep(1000);	
		Telnet_Control("\x8\x1b[\xff\x50%d",i);			
	}
	Telnet_Control("\r\n");		
	soft_reset();	
	
#ifdef	OS_UCOS
	OSIntExit();  	
#endif 
}
