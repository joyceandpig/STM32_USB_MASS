#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


 
#ifndef NULL
#define NULL 0
#endif

//�ڴ�����趨.
#define MEM_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE			35*1024  						//�������ڴ� 35K
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//�ڴ����С
 
		 
//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(void);				//��ʼ��
	u8 (*perused)(void);		  	//�ڴ�ʹ����
	u8 	*membase;					//�ڴ�� 
	u16 *memmap; 					//�ڴ����״̬��
	u8  memrdy; 					//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��

void mal_mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mal_mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mal_mem_init(void);					 //�ڴ������ʼ������(��/�ڲ�����)
u32 mal_mem_malloc(u32 size);		 		//�ڴ����(�ڲ�����)
u8 mal_mem_free(u32 offset);		 		//�ڴ��ͷ�(�ڲ�����)
u8 mal_mem_perused(void);					//���ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void mal_myfree(void *ptr);  				//�ڴ��ͷ�(�ⲿ����)
void *mal_mymalloc(u32 size);				//�ڴ����(�ⲿ����)
void *mal_myrealloc(void *ptr,u32 size);	//���·����ڴ�(�ⲿ����)
#endif












