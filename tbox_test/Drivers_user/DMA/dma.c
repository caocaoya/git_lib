#include "dma.h"
#include "spi.h"
#include "usart.h"

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void DMA_Config(DMA_HandleTypeDef  DMA_Handhler_DEF,DMA_Channel_TypeDef *DMA_Streamx)
{ 
	
		if((u32)DMA_Streamx > (u32)DMA2)//�õ���ǰstream������DMA2����DMA1
		{
			__HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
		}
		else 
		{
		 __HAL_RCC_DMA1_CLK_ENABLE();//DMA1ʱ��ʹ�� 
		}   
    
		// DMA����
		DMA_Handhler_DEF.Instance=DMA_Streamx;                            //������ѡ��
		
		DMA_Handhler_DEF.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
		
		DMA_Handhler_DEF.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
		DMA_Handhler_DEF.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
		DMA_Handhler_DEF.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
		DMA_Handhler_DEF.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
		DMA_Handhler_DEF.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
		DMA_Handhler_DEF.Init.Priority=DMA_PRIORITY_LOW;               //�е����ȼ�
		
		HAL_DMA_Init(&DMA_Handhler_DEF);
		
} 

void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t Size)
{
   HAL_DMA_Start(huart->hdmatx, (u32)pData,(uint32_t)&huart->Instance->DR, Size);
	
   huart->Instance->CR3 |= USART_CR3_DMAT;
}
