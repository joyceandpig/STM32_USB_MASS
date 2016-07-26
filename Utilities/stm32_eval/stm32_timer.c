#include "my_platform.h"

void Timer2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef RCC_InitStructure;
	u16 tmp = 0;
	float period;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IRQn_Priority;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
	TIM_TimeBaseStructure.TIM_Period        = 200-1;  //�Զ���װֵ,ֵԽ�� ����Խ�� Ĭ��
	TIM_TimeBaseStructure.TIM_Prescaler     = 36-1;   //��Ƶ��,ֵԽ�� ����Խ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;       //ʱ�ӷ�Ƶϵ��
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //���ϼ���
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
	u_printf(INFO,"TIM2 CLK Frequency:%dMhz.",tmp);//��ӡTimer ʱ��	
	period = (TIM_TimeBaseStructure.TIM_Prescaler+1) * (TIM_TimeBaseStructure.TIM_Period+1)/(tmp*1.0);//���� = ((TIM_Prescaler+1) * (TIM_Period+1)/(f_timer(hz)*1.0))(us);
	u_printf(INFO,"TIM2 Period = %.2fus, Freq = %.2fHz.",period,1000000.0/period);//�����ʱ����
		
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���жϱ�־
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ������ж�
	TIM_Cmd(TIM2,ENABLE);         	//Timer
	u_printf(INFO,"TIM2 Init is Successful.");
}
extern void DZ_Application(void);//���붨ʱ��������
void TIM2_IRQHandler(void)
{
#ifdef	OS_UCOS	
	OSIntEnter(); //�жϱ���
#endif
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���жϱ�־
	}	
#ifdef	OS_UCOS
	OSIntExit();  
#endif
}


