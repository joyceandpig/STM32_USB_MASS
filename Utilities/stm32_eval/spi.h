#ifndef __SPI_H
#define __SPI_H
#include "my_platform.h"


// SPI�����ٶ����� 
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3
						  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

