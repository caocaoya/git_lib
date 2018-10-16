#include "usart.h"
#include "delay.h"
#include "string.h"
#define  DMA_UART3     1

UART_HandleTypeDef   MCU_M720_UART1_Handler; //UART句柄
UART_HandleTypeDef   MCU_T_BOX_UART2_Handler; //UART句柄
UART_HandleTypeDef   MCU_GPS_UART3_Handler; //UART句柄
DMA_HandleTypeDef    DMA_UART3_TX_Handler;
DMA_HandleTypeDef    DMA_UART3_RX_Handler;	


//接收状态
//USART_RX_STA_M720
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u8  USART_RX_BUF_M720[USART_REC_LEN_M720];   //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART_RX_STA_M720;         		           //接收状态标记	
u8  USART_RX_BUF_GPS[USART_REC_LEN_GPS];     //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART_RX_STA_GPS;         		           //接收状态标记	
u8  USART_RX_BUF_T_BOX[USART_REC_LEN_T_BOX]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART_RX_STA_T_BOX;         		         //接收状态标记	


u8 aRxBuffer_M720[RXBUFFERSIZE_M720];//HAL库使用的串口接收缓冲
u8 aRxBuffer_GPS[RXBUFFERSIZE_GPS];//HAL库使用的串口接收缓冲
u8 aRxBuffer_T_BOX[RXBUFFERSIZE_T_BOX];//HAL库使用的串口接收缓冲

void Uart_Init(UART_HandleTypeDef *Uart_Handler, u32 bound)
{
	USART_TypeDef *uart_x;
	
	if((Uart_Handler) == (&MCU_M720_UART1_Handler))
	{
		uart_x = USART1;
	}
	else if((Uart_Handler) == (&MCU_T_BOX_UART2_Handler))
	{
		uart_x = USART2;
	}
	else if((Uart_Handler) == (&MCU_GPS_UART3_Handler))
	{
		uart_x = USART3;
	}
	
	(*Uart_Handler).Instance          = uart_x;
  (*Uart_Handler).Init.BaudRate     = bound;
  (*Uart_Handler).Init.WordLength   = UART_WORDLENGTH_8B;
  (*Uart_Handler).Init.StopBits     = UART_STOPBITS_1;
  (*Uart_Handler).Init.Parity       = UART_PARITY_NONE;
  (*Uart_Handler).Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  (*Uart_Handler).Init.Mode         = UART_MODE_TX_RX;
  (*Uart_Handler).Init.OverSampling = UART_OVERSAMPLING_16;
	
  HAL_UART_Init(Uart_Handler);
	
	if((Uart_Handler) == (&MCU_M720_UART1_Handler))
	{
	  HAL_UART_Receive_IT(Uart_Handler, (u8 *)aRxBuffer_M720, RXBUFFERSIZE_M720);
	}
	else if((Uart_Handler) == (&MCU_T_BOX_UART2_Handler))
	{
		HAL_UART_Receive_IT(Uart_Handler, (u8 *)aRxBuffer_T_BOX, RXBUFFERSIZE_T_BOX);
	}
	else if((Uart_Handler) == (&MCU_GPS_UART3_Handler))
	{
	#if DMA_UART3
 
    HAL_UART_Receive_DMA(Uart_Handler,(u8 *)aRxBuffer_GPS,RXBUFFERSIZE_GPS);	
		__HAL_UART_ENABLE_IT(&MCU_GPS_UART3_Handler, UART_IT_IDLE);
		__HAL_UART_DISABLE_IT(&MCU_GPS_UART3_Handler, UART_IT_RXNE);
  
	#else
		
    HAL_UART_Receive_IT(Uart_Handler, (u8 *)aRxBuffer_GPS, RXBUFFERSIZE_GPS);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
		
	#endif		
	}
	
	
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;

	if(huart->Instance==USART1)
	{
	  MCU_M720_UART_GPIO_CLK();			                 //使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			           //使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=MCU_M720_UART_TX_PIN;			  
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		         //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			           //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;       //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	

		GPIO_Initure.Pin=MCU_M720_UART_RX_PIN;			    
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	            //模式要设置为复用输入模式！	
		HAL_GPIO_Init(MCU_M720_UART_PART,&GPIO_Initure);//初始化PA10
#if 1
		HAL_NVIC_EnableIRQ(MCU_M720_UART_IRQ);				  //使能USART1中断通道
		HAL_NVIC_SetPriority(MCU_M720_UART_IRQ,MCU_M720_UART_IRQ_PREEMPTPRIORTY,MCU_M720_UART_IRQ_SUBPRIORTY);
#endif	
	}
	
	if(huart->Instance==USART2)
	{
		MCU_T_BOX_UART_GPIO_CLK();			                //使能GPIO时钟
		__HAL_RCC_USART2_CLK_ENABLE();			            //使能USART2时钟
	  __HAL_RCC_AFIO_CLK_ENABLE();
		
		GPIO_Initure.Pin=MCU_T_BOX_UART_TX_PIN;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		          //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			            //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		    //高速

		HAL_GPIO_Init(MCU_T_BOX_UART_PART,&GPIO_Initure);//初始化PA9

		GPIO_Initure.Pin=MCU_T_BOX_UART_RX_PIN;		
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	             //模式要设置为复用输入模式！
		HAL_GPIO_Init(MCU_T_BOX_UART_PART,&GPIO_Initure);	   	
		
#if 1
		HAL_NVIC_EnableIRQ(MCU_T_BOX_UART_IRQ);				   //使能USART2中断通道
		HAL_NVIC_SetPriority(MCU_T_BOX_UART_IRQ,MCU_T_BOX_UART_IRQ_PREEMPTPRIORTY,MCU_T_BOX_UART_IRQ_SUBPRIORTY);
#endif	
	}
	
	if(huart->Instance==USART3)
	{	
		MCU_GPS_UART_GPIO_CLK();			                   //使能GPIO时钟
	  __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();		               //使能USART3时钟
  	__HAL_AFIO_REMAP_USART3_ENABLE();
		
		
		GPIO_Initure.Pin=MCU_GPS_UART_TX_PIN;		 
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		           //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			             //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	       //高速

		HAL_GPIO_Init(MCU_GPS_UART_PART,&GPIO_Initure);	   	

		GPIO_Initure.Pin=MCU_GPS_UART_RX_PIN;			 
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	             //模式要设置为复用输入模式！
		HAL_GPIO_Init(MCU_GPS_UART_PART,&GPIO_Initure);	 
 	
#if DMA_UART3
		
		__HAL_RCC_DMA1_CLK_ENABLE();

		DMA_UART3_RX_Handler.Instance                 = DMA1_Channel3;
		DMA_UART3_RX_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		DMA_UART3_RX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
		DMA_UART3_RX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
		DMA_UART3_RX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		DMA_UART3_RX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		DMA_UART3_RX_Handler.Init.Mode                = DMA_NORMAL;//DMA_NORMAL   DMA_CIRCULAR
		DMA_UART3_RX_Handler.Init.Priority            = DMA_PRIORITY_LOW;

		HAL_DMA_Init(&DMA_UART3_RX_Handler);

		__HAL_LINKDMA(huart, hdmarx, DMA_UART3_RX_Handler);
		
		DMA_UART3_TX_Handler.Instance                 = DMA1_Channel2;
		DMA_UART3_TX_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		DMA_UART3_TX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
		DMA_UART3_TX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
		DMA_UART3_TX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		DMA_UART3_TX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		DMA_UART3_TX_Handler.Init.Mode                = DMA_NORMAL;
		DMA_UART3_TX_Handler.Init.Priority            = DMA_PRIORITY_LOW;

		HAL_DMA_Init(&DMA_UART3_TX_Handler);
		
		__HAL_LINKDMA(huart, hdmatx, DMA_UART3_TX_Handler);
		
	  HAL_NVIC_SetPriority(UART3_DMA_TX_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(UART3_DMA_TX_IRQn);
		
		HAL_NVIC_SetPriority(UART3_DMA_RX_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(UART3_DMA_RX_IRQn);
#endif 

#if 1
		HAL_NVIC_EnableIRQ(MCU_GPS_UART_IRQ);				//使能USART1中断通道
		HAL_NVIC_SetPriority(MCU_GPS_UART_IRQ,MCU_GPS_UART_IRQ_PREEMPTPRIORTY,MCU_GPS_UART_IRQ_SUBPRIORTY);			//抢占优先级3，子优先级3
#endif	
	}

}
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(MCU_M720_UART_PART, MCU_GPS_UART_TX_PIN | MCU_GPS_UART_RX_PIN);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
  else if(huart->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();
    HAL_GPIO_DeInit(MCU_T_BOX_UART_PART, MCU_T_BOX_UART_TX_PIN | MCU_T_BOX_UART_RX_PIN);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
  else if(huart->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();
    HAL_GPIO_DeInit(MCU_GPS_UART_PART, MCU_GPS_UART_TX_PIN | MCU_GPS_UART_RX_PIN);
    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA_M720 & 0x8000)==0)//接收未完成
		{
			if(USART_RX_STA_M720&0x4000)//接收到了0x0d
			{
				if(aRxBuffer_M720[0]!=0x0a)USART_RX_STA_M720=0;//接收错误,重新开始
				else USART_RX_STA_M720|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer_M720[0]==0x0d)USART_RX_STA_M720|=0x4000;
				else
				{
					USART_RX_BUF_M720[USART_RX_STA_M720&0X3FFF]=aRxBuffer_M720[0] ;
					USART_RX_STA_M720++;
					if(USART_RX_STA_M720>(USART_REC_LEN_M720-1))USART_RX_STA_M720=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
		else
		{
			USART_RX_STA_M720=0;
		}
	}
	
	if(huart->Instance==USART2)//如果是串口2
	{
		if((USART_RX_STA_T_BOX & 0x8000)==0)//接收未完成
		{
			if(USART_RX_STA_T_BOX&0x4000)//接收到了0x0d
			{
				if(aRxBuffer_T_BOX[0]!=0x0a)USART_RX_STA_T_BOX=0;//接收错误,重新开始
				else USART_RX_STA_T_BOX|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer_T_BOX[0]==0x0d)USART_RX_STA_T_BOX|=0x4000;
				else
				{
					USART_RX_BUF_T_BOX[USART_RX_STA_T_BOX&0X3FFF]=aRxBuffer_T_BOX[0] ;
					USART_RX_STA_T_BOX++;
					if(USART_RX_STA_T_BOX>(USART_REC_LEN_T_BOX-1))USART_RX_STA_T_BOX=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
		else
		{	
			USART_RX_STA_T_BOX=0;			
		}
	}
	
	if(huart->Instance==USART3)//如果是串口3
	{
		#if DMA_UART3 
		UsartReceive_IDLE(&MCU_GPS_UART3_Handler);
		#else
		
	  if((USART_RX_STA_GPS&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA_GPS&0x4000)//接收到了0x0d
			{
				if(aRxBuffer_GPS[0]!=0x0a)USART_RX_STA_GPS=0;//接收错误,重新开始
				else USART_RX_STA_GPS|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer_GPS[0]==0x0d)USART_RX_STA_GPS|=0x4000;
				else
				{
					USART_RX_BUF_GPS[USART_RX_STA_GPS&0X3FFF]=aRxBuffer_GPS[0] ;
					USART_RX_STA_GPS++;
					if(USART_RX_STA_GPS>(USART_REC_LEN_GPS-1))USART_RX_STA_GPS=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
		else
		{		
			USART_RX_STA_GPS=0;
		}
    #endif
	}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	uint32_t uart_error_flag = huart->ErrorCode;
	
	if(huart->Instance==USART1)//如果是串口1
	{
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			CLEAR_BIT(huart->ErrorCode, HAL_UART_ERROR_ORE);
		}
    
	}
	if(huart->Instance==USART2)//如果是串口2
	{		
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			CLEAR_BIT(huart->ErrorCode, HAL_UART_ERROR_ORE);
		}
	}
	if(huart->Instance==USART3)//如果是串口3
	{		
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
			__HAL_UART_CLEAR_OREFLAG(huart);
		}	
	}
}

//串口空闲中断处理函数
void UsartReceive_IDLE(UART_HandleTypeDef *huart)
{	 
	uint16_t temp = 0;
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))	
  { 	
    __HAL_UART_CLEAR_IDLEFLAG(huart);		
    HAL_UART_DMAStop(huart);
	
		temp = __HAL_DMA_GET_COUNTER(huart->hdmarx);
		USART_RX_STA_GPS = RXBUFFERSIZE_GPS - temp;	
		huart->hdmarx->Instance->CNDTR = 0;
		
		memcpy(USART_RX_BUF_GPS,aRxBuffer_GPS,USART_RX_STA_GPS);
		
		USART_RX_STA_GPS |= 0x8000;			
		if((USART_RX_STA_GPS & 0x8000) != 0x8000)
		{
			HAL_UART_Receive_DMA(huart,aRxBuffer_GPS,RXBUFFERSIZE_GPS);
		}
  }
}

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&MCU_M720_UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
  while(HAL_UART_GetState(&MCU_M720_UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	   timeout++;////超时处理
     if(timeout>maxDelay) break;			
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&MCU_M720_UART1_Handler, (u8 *)aRxBuffer_M720, RXBUFFERSIZE_M720) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>maxDelay) break;	
	}
} 

//串口2中断服务程序
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&MCU_T_BOX_UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
  while(HAL_UART_GetState(&MCU_T_BOX_UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	   timeout++;////超时处理
     if(timeout>maxDelay) break;	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&MCU_T_BOX_UART2_Handler, (u8 *)aRxBuffer_T_BOX, RXBUFFERSIZE_T_BOX) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>maxDelay) break;	
	}
}

//串口3中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
#if DMA_UART3
//	UsartReceive_IDLE(&MCU_GPS_UART3_Handler);
#endif	
	HAL_UART_IRQHandler(&MCU_GPS_UART3_Handler);	
	
#if DMA_UART3	
	
//	HAL_UART_Receive_DMA(&MCU_GPS_UART3_Handler, (u8 *)aRxBuffer_GPS, RXBUFFERSIZE_GPS);

#else
	timeout=0;
  while(HAL_UART_GetState(&MCU_GPS_UART3_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	   timeout++;////超时处理
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
  while(HAL_UART_Receive_IT(&MCU_GPS_UART3_Handler, (u8 *)aRxBuffer_GPS, RXBUFFERSIZE_GPS) != HAL_OK)	
	{
	 timeout++; //超时处理
	 if(timeout>maxDelay) break;	
	}
#endif
}

#if DMA_UART3
void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&DMA_UART3_TX_Handler);
}
void DMA1_Channel3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&DMA_UART3_RX_Handler);
}
#endif


HAL_StatusTypeDef UART_Send_messag(UART_HandleTypeDef *huart, uint8_t *p_string, uint8_t size, uint32_t Timeout)
{
    if(!size )
    {
         while (p_string[size] != '\0')
        {
          size++;
        }
    }
  return HAL_UART_Transmit(huart, p_string, size, Timeout);
}


void UART_Recevie_messag(UART_HandleTypeDef *huart, uint8_t *p_string, uint16_t size, uint32_t Timeout)
{
   HAL_UART_Receive(huart, p_string, size, Timeout); 
}

void M720_Info_Req(uint8_t *type, uint8_t *key, uint16_t size)
{
   UART_Send_messag(&MCU_M720_UART1_Handler, type, 0, MCU_M720_TX_TIME);
   UART_Recevie_messag(&MCU_M720_UART1_Handler, key, size, MCU_M720_RX_TIME);
} 


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
