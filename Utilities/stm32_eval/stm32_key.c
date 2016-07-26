#include "my_platform.h"

#define LINE_MAX		3
#define ROW_MAX			3
         
const struct IO_PORT key_output[LINE_MAX] = {	\
	{RCC_APB2Periph_GPIOA,GPIOA, GPIO_Pin_0}, 	\
	{RCC_APB2Periph_GPIOA,GPIOA, GPIO_Pin_1}, 	\
	{RCC_APB2Periph_GPIOA,GPIOA, GPIO_Pin_8}, 	\
};  
const struct IO_PORT key_input[ROW_MAX] = 	{		\
	{RCC_APB2Periph_GPIOC,GPIOC, GPIO_Pin_4},	\
	{RCC_APB2Periph_GPIOC,GPIOC, GPIO_Pin_5}, 	\
	{RCC_APB2Periph_GPIOC,GPIOC, GPIO_Pin_6}, 	\
};
		
unsigned char key[LINE_MAX][ROW_MAX];

void user_key_init(void)
{
	u8 i;
	GPIO_InitTypeDef  GPIO_InitStructure;
			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//IO���ٶ�Ϊ50MHz
	for(i= 0 ;i<LINE_MAX;i++)
	{
		RCC_APB2PeriphClockCmd(key_output[i].RCC_APB2Periph, ENABLE);		//ʹ�ܶ˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = key_output[i].GPIO_pin ;					//�˿�����
		GPIO_Init(key_output[i].GPIO_x , &GPIO_InitStructure);					//�����趨������ʼ��	
	}	
	/* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */ 
	/* PA4 PA5 PA6����*/  
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	for(i= 0 ;i<ROW_MAX;i++)
	{	
		RCC_APB2PeriphClockCmd(key_input[i].RCC_APB2Periph, ENABLE);		//ʹ�ܶ˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = key_input[i].GPIO_pin;						//�˿�����           
		GPIO_Init(key_input[i].GPIO_x, &GPIO_InitStructure);						//�����趨������ʼ��	   
	}		
	
	for(i = 0; i < LINE_MAX; i++) 
	{         
		GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);  
	}
	
	u_printf(INFO,"The Buttons Init is Sucessful.");
}

u8 update_key(void) 
{  
	u8 key_val = 0;
	static u8 last_key_val = 0;
	static u16 cnt = 0;
	unsigned char i, j;
	for(i = 0; i < LINE_MAX; i++)             //i������ڣ������õ͵�ƽ 
	{    
		GPIO_ResetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin);       
		for(j = 0; j < ROW_MAX; j++)            //j������ڣ���������ʱ��ͨ����Ϊ�͵�ƽ     
		{      
			if(GPIO_ReadInputDataBit(key_input[j].GPIO_x, key_input[j].GPIO_pin) == 0)      
			{  
				key[i][j] = 1; 
				key_val = i*3+j+1;
				if(last_key_val == key_val)
				{//��סû���ͷ�
					cnt ++;
					if(cnt > 20)
					{//����
						u_printf(DBG,"Key!%d",key_val);
						last_key_val = 0;
						cnt =0;
						return (key_val);								
					}
					else
					{
						last_key_val = key_val;
					}
				}
				else
				{//��¼��ֵ
						cnt = 0;
						last_key_val = key_val;
				}					
			}  
			else 
			{  				
					key[i][j] = 0; 
					key_val = 0;				
			}     
		}    
		GPIO_SetBits(key_output[i].GPIO_x, key_output[i].GPIO_pin); 
	} 
	return 0;	
}

