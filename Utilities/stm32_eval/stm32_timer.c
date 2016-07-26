#include "my_platform.h"

void Timer2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef RCC_InitStructure;
	u16 tmp = 0;
	float period;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IRQn_Priority;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
	TIM_TimeBaseStructure.TIM_Period        = 200-1;  //自动重装值,值越大 周期越长 默认
	TIM_TimeBaseStructure.TIM_Prescaler     = 36-1;   //分频数,值越大 周期越大
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;       //时钟分频系数
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  

	RCC_GetClocksFreq(&RCC_InitStructure);
	if((RCC->CFGR & (uint32_t)0x0400) == 0)//div1
	{
		tmp = RCC_InitStructure.PCLK1_Frequency/1000000;
	}		
	else
	{
		tmp = 2*RCC_InitStructure.PCLK1_Frequency/1000000;
	}	
	u_printf(INFO,"TIM2 CLK Frequency:%dMhz.",tmp);//打印Timer 时钟	
	period = (TIM_TimeBaseStructure.TIM_Prescaler+1) * (TIM_TimeBaseStructure.TIM_Period+1)/(tmp*1.0);//周期 = ((TIM_Prescaler+1) * (TIM_Period+1)/(f_timer(hz)*1.0))(us);
	u_printf(INFO,"TIM2 Period = %.2fus, Freq = %.2fHz.",period,1000000.0/period);//输出定时周期
		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能溢出中断
	TIM_Cmd(TIM2,ENABLE);         	//Timer
	u_printf(INFO,"TIM2 Init is Successful.");
}
extern void DZ_Application(void);//电针定时器处理函数
void TIM2_IRQHandler(void)
{
#ifdef	OS_UCOS	
	OSIntEnter(); //中断保护
#endif
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清中断标志
	}	
#ifdef	OS_UCOS
	OSIntExit();  
#endif
}


