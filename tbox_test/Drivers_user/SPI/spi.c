#include "spi.h"
#include "dma.h"


#define DMA_SPI 0  //SPI是否需要DMA传输

SPI_HandleTypeDef SPI1_Handler;  //SPI句柄
DMA_HandleTypeDef DMA_SP1_TX_Handler;
DMA_HandleTypeDef DMA_SP1_RX_Handler;

void SPI1_Init(void)
{
    SPI1_Handler.Instance=SPIx;                         //SP5
    SPI1_Handler.Init.Mode=SPI_MODE_SLAVE;             //设置SPI工作模式，设置为主模式     //SPI_MODE_SLAVE    SPI_MODE_MASTER
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
	
    SPI1_ReadWriteByte(0Xff);                           //启动传输
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
      __HAL_RCC_DMA1_CLK_ENABLE();//DMA1时钟使能 

			// DMA配置
			DMA_SP1_TX_Handler.Instance=DMA1_Channel3;                          //数据流选择
			DMA_SP1_TX_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
			DMA_SP1_TX_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
			DMA_SP1_TX_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
			DMA_SP1_TX_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
			DMA_SP1_TX_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
			DMA_SP1_TX_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
			DMA_SP1_TX_Handler.Init.Priority=DMA_PRIORITY_LOW;                  //中等优先级
			HAL_DMA_Init(&DMA_SP1_TX_Handler);
			
			__HAL_LINKDMA(hspi, hdmatx, DMA_SP1_TX_Handler);
		
			// DMA配置
			DMA_SP1_RX_Handler.Instance=DMA1_Channel2;                          //数据流选择
			DMA_SP1_RX_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
			DMA_SP1_RX_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
			DMA_SP1_RX_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
			DMA_SP1_RX_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
			DMA_SP1_RX_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
			DMA_SP1_RX_Handler.Init.Mode=DMA_NORMAL;                            //外设普通模式
			DMA_SP1_RX_Handler.Init.Priority=DMA_PRIORITY_HIGH;               //中等优先级
			
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
//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为45Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
    
}

//SPI5 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}
