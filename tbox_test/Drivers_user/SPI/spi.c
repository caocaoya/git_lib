#include "spi.h"
#include "dma.h"


#define DMA_SPI 0  //SPI�Ƿ���ҪDMA����

SPI_HandleTypeDef SPI1_Handler;  //SPI���
DMA_HandleTypeDef DMA_SP1_TX_Handler;
DMA_HandleTypeDef DMA_SP1_RX_Handler;

void SPI1_Init(void)
{
    SPI1_Handler.Instance=SPIx;                         //SP5
    SPI1_Handler.Init.Mode=SPI_MODE_SLAVE;             //����SPI����ģʽ������Ϊ��ģʽ     //SPI_MODE_SLAVE    SPI_MODE_MASTER
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //ʹ��SPI1
	
    SPI1_ReadWriteByte(0Xff);                           //��������
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    if(hspi->Instance == SPI1 )
		{
			SPIx_NSS_GPIO_CLK_ENABLE();
			SPIx_SCK_GPIO_CLK_ENABLE();
			SPIx_MISO_GPIO_CLK_ENABLE();
			SPIx_MOSI_GPIO_CLK_ENABLE();
			__HAL_RCC_AFIO_CLK_ENABLE();
			SPIx_CLK_ENABLE();

			GPIO_Initure.Pin       = SPIx_SCK_PIN;
			GPIO_Initure.Mode      = GPIO_MODE_AF_PP;
			GPIO_Initure.Pull      = GPIO_PULLUP;
			GPIO_Initure.Speed     = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_Initure);

			GPIO_Initure.Pin = SPIx_MISO_PIN;
			HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_Initure);

			GPIO_Initure.Pin = SPIx_MOSI_PIN;
			HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_Initure);

			GPIO_Initure.Pin       = SPIx_NSS_PIN;
			GPIO_Initure.Mode      = GPIO_MODE_INPUT;
			GPIO_Initure.Pull      = GPIO_NOPULL;
			GPIO_Initure.Speed     = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_Initure);
//			SPI_CS_HIGH();
			
#if (DMA_SPI == 1)
      __HAL_RCC_DMA1_CLK_ENABLE();//DMA1ʱ��ʹ�� 

			// DMA����
			DMA_SP1_TX_Handler.Instance=DMA1_Channel3;                          //������ѡ��
			DMA_SP1_TX_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
			DMA_SP1_TX_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
			DMA_SP1_TX_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
			DMA_SP1_TX_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
			DMA_SP1_TX_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
			DMA_SP1_TX_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
			DMA_SP1_TX_Handler.Init.Priority=DMA_PRIORITY_LOW;                  //�е����ȼ�
			HAL_DMA_Init(&DMA_SP1_TX_Handler);
			
			__HAL_LINKDMA(hspi, hdmatx, DMA_SP1_TX_Handler);
		
			// DMA����
			DMA_SP1_RX_Handler.Instance=DMA1_Channel2;                          //������ѡ��
			DMA_SP1_RX_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
			DMA_SP1_RX_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
			DMA_SP1_RX_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
			DMA_SP1_RX_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
			DMA_SP1_RX_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
			DMA_SP1_RX_Handler.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
			DMA_SP1_RX_Handler.Init.Priority=DMA_PRIORITY_HIGH;               //�е����ȼ�
			
			HAL_DMA_Init(&DMA_SP1_RX_Handler);
			__HAL_LINKDMA(hspi, hdmarx, DMA_SP1_RX_Handler);
			
			HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 5, 0);
			HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);
			
			HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 6, 0);
			HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);
#endif				
		}

}

#if (DMA_SPI == 1)
void SPIx_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(SPI1_Handler.hdmarx);
}

void SPIx_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(SPI1_Handler.hdmatx);
}

#endif
//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ45Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI1_Handler);            //�ر�SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI1_Handler);             //ʹ��SPI
    
}

//SPI5 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}
