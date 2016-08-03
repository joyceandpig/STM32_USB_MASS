#include "my_platform.h"

/*USART*/
USART_InitTypeDef 	\
USART_COM1 ={	57600,	\
							USART_WordLength_8b,						\
							USART_StopBits_1,								\
							USART_Parity_No,								\
							USART_Mode_Rx | USART_Mode_Tx,	\
							USART_HardwareFlowControl_None	\
						};/*COM1 ��������*/
USART_InitTypeDef 	\
USART_COM2 ={	115200,	\
							USART_WordLength_8b,						\
							USART_StopBits_1,								\
							USART_Parity_No,								\
							USART_Mode_Rx | USART_Mode_Tx,	\
							USART_HardwareFlowControl_None	\
						};/*COM2 ��������*/

USART_TypeDef* 	COM_USART[COMn] 				= 	{EVAL_COM1, 							EVAL_COM2}; 
GPIO_TypeDef* 	COM_TX_PORT[COMn] 			= 	{EVAL_COM1_TX_GPIO_PORT, 	EVAL_COM2_TX_GPIO_PORT};
GPIO_TypeDef* 	COM_RX_PORT[COMn] 			= 	{EVAL_COM1_RX_GPIO_PORT, 	EVAL_COM2_RX_GPIO_PORT};
const uint32_t 	COM_USART_CLK[COMn] 		= 	{EVAL_COM1_CLK, 					EVAL_COM2_CLK};
const uint32_t 	COM_TX_PORT_CLK[COMn] 	= 	{EVAL_COM1_TX_GPIO_CLK, 	EVAL_COM2_TX_GPIO_CLK};
const uint32_t 	COM_RX_PORT_CLK[COMn] 	= 	{EVAL_COM1_RX_GPIO_CLK, 	EVAL_COM2_RX_GPIO_CLK};
const uint16_t 	COM_TX_PIN[COMn] 				= 	{EVAL_COM1_TX_PIN, 				EVAL_COM2_TX_PIN};
const uint16_t 	COM_RX_PIN[COMn] 				= 	{EVAL_COM1_RX_PIN, 				EVAL_COM2_RX_PIN};
const uint16_t 	COM_IRQn[COMn]					=		{USART1_IRQn,							USART2_IRQn};
const uint16_t 	COM_IRQn_Priority[COMn]	=		{USART1_IRQn_Priority,		USART2_IRQn_Priority};
 
#include "stdio.h"
/*�������´���,֧��printf����,������Ҫѡ��use MicroLIB	*/  
#if defined(__MICROLIB)
	#warning	"Use MicroLIB Is Enable! "
#else
//	#warning	"Use MicroLIB Is Disable! "            
	//��׼����Ҫ��֧�ֺ���                 
	struct __FILE 
	{ 
		int handle; 

	}; 

	FILE __stdout;       
	//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
	void _sys_exit(int x) 
	{ 
		x = x; 
	} 
	//�ض���fputc���� 
	int fputc(int ch, FILE *f)
	{ 
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
		USART1->DR = (u8) ch;    
		return ch;
	}
#endif 
	

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  if (COM == COM1)
  {
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
  }
  else
  {
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = COM_IRQn[COM];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = COM_IRQn_Priority[COM] ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
	
	USART_ITConfig(COM_USART[COM], USART_IT_RXNE, ENABLE);//�������ڽ����ж�  
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
#if 0
	u_printf(INFO,"Serial Port COM%d Init is Successful.\r\n \tBaudRate[%ld],DataBits[%s],Parity[%s],StopBits[%s],FlowControl[%s]",COM+1,\
						USART_InitStruct->USART_BaudRate,\
						(USART_InitStruct->USART_WordLength == USART_WordLength_8b)?"8Bit":"9Bit",\
						(USART_InitStruct->USART_Parity == USART_Parity_Odd)?"Odd":(USART_InitStruct->USART_Parity == USART_Parity_Even)?"Even":"None",	\
						(USART_InitStruct->USART_StopBits == USART_StopBits_1)?"1":(USART_InitStruct->USART_StopBits == USART_StopBits_1_5)?"1.5":(USART_InitStruct->USART_StopBits == USART_StopBits_2)?"2":"0.5",\
						(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_RTS)?"RTS":(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_CTS)?"CTS":\
						(USART_InitStruct->USART_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS)?"RTS&CTS":"None"\
						);
#endif

}

void SoftReset(void)
{//�����λ
	u_printf(WARN,"System well Reset.\n");
	NVIC_SystemReset();
  __DSB();  
	while (1);
}

#define USART_RECV_LEN  			4096  	//�����������ֽ��� 512
char USART_RX_BUF[USART_RECV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u16 USART_Read_Data(char *buf)
{
	u16 ret;
	if(USART_RX_STA & 0x8000)
	{
		ret = USART_RX_STA&0x3fff;
		strcpy(buf,(const char *)&USART_RX_BUF);
		USART_RX_STA = 0x0000;
		memset(USART_RX_BUF,0,sizeof(char)*USART_RECV_LEN);//����ڴ�			
	}
	else
		ret =0;
	
	return ret;
}
u8 CMD_FLAG = 0;	
static void Recv_Process(void)
{
	s8 Res;
	static u8 EscFlag = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);

		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������	
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(EscFlag == 0)
			{//û�յ���ת���ַ�
				if(Res >= 32 && Res <=127)
				{//��Ч�ַ�							
					
					USART_SendData(USART1,Res);	//printf("%c",Res);	
					
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_RECV_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
				}					
				else
				{//�����ַ�
					if(Res == 0x0d)
					{//�س�						
						if((USART_RX_STA&0X3FFF) != 0)
							USART_RX_STA |= 0x8000;	//���������
						printf("\r\n[sunsheen@Terminal]");		

						CMD_FLAG = 1;					 
					}									
					else if(Res == 0x8)
					{								
						if(USART_RX_STA == 0)
							USART_RX_STA = 0;
						else
						{
							printf("%c\x1b[\xff\x50",Res);	//��ɾһ���ַ�
							USART_RX_STA--;
						}
						USART_RX_BUF[USART_RX_STA&0X3FFF]=0 ;					
					}
					else if(Res == 0x1b)//ESC
					{//���յ�ת���ַ�
						EscFlag =1;
					}
				}
			}
			else
			{//ת���ַ�  �������� "\[A"
				static u8 i =0;
				if(++i == 2)
				{
					i = 0;
					EscFlag = 0;
				}				
			}					
		}			
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
		{//�ȶ�SR �ٶ�DR ������������־λ
			USART_ClearFlag(USART1,USART_FLAG_ORE);        //��SR
			Res = USART_ReceiveData(USART1);               //��DR
			u_printf(WARN,"COM1 Receive is Overflow!");
		}			
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
	{//�ȶ�SR �ٶ�DR ������������־λ
		USART_ClearFlag(USART1,USART_FLAG_ORE);        //��SR
		Res = USART_ReceiveData(USART1);               //��DR
		u_printf(WARN,"COM1 Receive is Overflow!");
	}
} 

void USART1_IRQHandler(void)                	//����1�жϷ������
{  
	u8 Res;
#ifdef	OS_UCOS
	OSIntEnter();  
#endif  
#ifndef USMART
	Recv_Process();
#else
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(4096-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#endif
	
#ifdef	OS_UCOS
	OSIntExit();  	
#endif  	
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
#ifdef	OS_UCOS
	OSIntEnter();   
#endif  	
	
	u_printf(ENTER);//���봮��2�ж�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		
		
	}	
	u_printf(EXIT);//�˳�����2�ж�
	
#ifdef	OS_UCOS
	OSIntExit(); 
#endif  	
}
	
