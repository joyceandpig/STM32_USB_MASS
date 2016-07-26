#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "type.h"
/*
*********************************************************************************************************
*                                            TASK PRIORITIES
* UCOS每个线程优先级不能相同，放在这里统一管理
*********************************************************************************************************
*/
#define WORK_QUEUE_MAX_SIZE 4
#ifdef OS_UCOS
enum TASK_PRIO{
	TASK_UNUSED_PRIO = 10,
	TASK_MOAL_WROK_PRIO,
	TASK_SDIO_THREAD_PRIO,	
	TASK_TCPIP_THREAD_PRIO,
	TASK_TIMER_TASKLET_PRIO,
	//TASK_MONITOR_PRIO,
	REASSOCIATION_THREAD_PRIO, 

	//以上顺序勿随意改动

	//user task
	TASK_MAIN_PRIO,



	

	OS_TASK_TMR_PRIO
};
#endif

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*单位WORD
*********************************************************************************************************
*/
#define TASK_TCPIP_THREAD_STACK_SIZE		512
#define TIMER_TASKLET_STACK_SIZE				256
#define MOAL_WROK_STACK_SIZE						256
#define TASK_MONITOR_STACK_SIZE					128
#define REASSOCIATION_THREAD_STACK_SIZE     400

//以上数值勿随意改动

#define TASK_MAIN_STACK_SIZE				512

/*
*********************************************************************************************************
*                               		IRQ_PRIORITY
*如果中断函数里需要打印信息，则此中断优先级要低于uart中断
*********************************************************************************************************
*/

enum IRQ_PRIORITY{
	EXTI2_IRQn_Priority,
	USART1_IRQn_Priority,
	USART2_IRQn_Priority,
	TIM2_IRQn_Priority,
	SDIO_IRQn_Priority,
	USART3_IRQn_Priority,
	DMA2_Stream5_IRQn_Priority,
	DMA2_Stream7_IRQn_Priority,
	ADC_IRQn_Priority,
	DMA1_Stream5_IRQn_Priority,
	SPI3_IRQn_Priority,
	DMA2_Stream0_IRQn_Priority,
	TIM3_IRQn_Priority,
	TIM4_IRQn_Priority,
	DMA2_Stream1_IRQn_Priority,
	DCMI_IRQn_Priority,
	SysTick_IRQn_Priority
};

/*
*********************************************************************************************************
*                                      kernel 里面常用资源值定义
*建议配合monitor软件确定合适的数值
*********************************************************************************************************
*/
#define EVENTS_MAX			48		//EVENTS_MAX是所有事件的数量之和，包括邮箱、消息队列、信号量、互斥量等
#define TIMER_MAX       		20		//定时器
#define MSG_QUEUE_MAX		16		//消息队列
#define TASKS_MAX			14		//任务
#define OS_TICK_RATE_HZ		100

/*
*********************************************************************************************************
*                                      定义中断向量表地址
* 因低16k地址用于bootloader，我们的代码需要从16k开始，所以需要在启动时将中断向量重新映射到16k地址
* 目前只对F205有效，F103版本不使用bootloader
*********************************************************************************************************
*/
//#define IVECTOR_ADDR 					(16*1024)
//#define IVECTOR_ADDR 					(0)		//如果不使用bootloaer（同时将IROM1的start改为0x8000000）

#endif
