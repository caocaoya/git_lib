#ifndef __DMA_H
#define __DMA_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//DMA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void DMA_Config(DMA_HandleTypeDef  DMA_Handhler_DEF,DMA_Channel_TypeDef *DMA_Streamx);
void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t Size);
#endif
