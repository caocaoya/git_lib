#include "dma.h"
#include "spi.h"
#include "usart.h"

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void DMA_Config(DMA_HandleTypeDef  DMA_Handhler_DEF,DMA_Channel_TypeDef *DMA_Streamx)
{ 
	
		if((u32)DMA_Streamx > (u32)DMA2)//得到当前stream是属于DMA2还是DMA1
		{
			__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	
		}
		else 
		{
		 __HAL_RCC_DMA1_CLK_ENABLE();//DMA1时钟使能 
		}   
    
		// DMA配置
		DMA_Handhler_DEF.Instance=DMA_Streamx;                            //数据流选择
		
		DMA_Handhler_DEF.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
		
		DMA_Handhler_DEF.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
		DMA_Handhler_DEF.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
		DMA_Handhler_DEF.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
		DMA_Handhler_DEF.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
		DMA_Handhler_DEF.Init.Mode=DMA_NORMAL;                            //外设普通模式
		DMA_Handhler_DEF.Init.Priority=DMA_PRIORITY_LOW;               //中等优先级
		
		HAL_DMA_Init(&DMA_Handhler_DEF);
		
} 

void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t Size)
{
   HAL_DMA_Start(huart->hdmatx, (u32)pData,(uint32_t)&huart->Instance->DR, Size);
	
   huart->Instance->CR3 |= USART_CR3_DMAT;
}
