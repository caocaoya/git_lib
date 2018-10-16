#include "usart.h"
#include "delay.h"
#include "string.h"
#define  DMA_UART3     1

UART_HandleTypeDef   MCU_M720_UART1_Handler; //UART���
UART_HandleTypeDef   MCU_T_BOX_UART2_Handler; //UART���
UART_HandleTypeDef   MCU_GPS_UART3_Handler; //UART���
DMA_HandleTypeDef    DMA_UART3_TX_Handler;
DMA_HandleTypeDef    DMA_UART3_RX_Handler;	


//����״̬
//USART_RX_STA_M720
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u8  USART_RX_BUF_M720[USART_REC_LEN_M720];   //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART_RX_STA_M720;         		           //����״̬���	
u8  USART_RX_BUF_GPS[USART_REC_LEN_GPS];     //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART_RX_STA_GPS;         		           //����״̬���	
u8  USART_RX_BUF_T_BOX[USART_REC_LEN_T_BOX]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART_RX_STA_T_BOX;         		         //����״̬���	


u8 aRxBuffer_M720[RXBUFFERSIZE_M720];//HAL��ʹ�õĴ��ڽ��ջ���
u8 aRxBuffer_GPS[RXBUFFERSIZE_GPS];//HAL��ʹ�õĴ��ڽ��ջ���
u8 aRxBuffer_T_BOX[RXBUFFERSIZE_T_BOX];//HAL��ʹ�õĴ��ڽ��ջ���

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
		
    HAL_UART_Receive_IT(Uart_Handler, (u8 *)aRxBuffer_GPS, RXBUFFERSIZE_GPS);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
		
	#endif		
	}
	
	
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;

	if(huart->Instance==USART1)
	{
	  MCU_M720_UART_GPIO_CLK();			                 //ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			           //ʹ��USART1ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=MCU_M720_UART_TX_PIN;			  
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		         //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			           //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;       //����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	

		GPIO_Initure.Pin=MCU_M720_UART_RX_PIN;			    
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	            //ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(MCU_M720_UART_PART,&GPIO_Initure);//��ʼ��PA10
#if 1
		HAL_NVIC_EnableIRQ(MCU_M720_UART_IRQ);				  //ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(MCU_M720_UART_IRQ,MCU_M720_UART_IRQ_PREEMPTPRIORTY,MCU_M720_UART_IRQ_SUBPRIORTY);
#endif	
	}
	
	if(huart->Instance==USART2)
	{
		MCU_T_BOX_UART_GPIO_CLK();			                //ʹ��GPIOʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			            //ʹ��USART2ʱ��
	  __HAL_RCC_AFIO_CLK_ENABLE();
		
		GPIO_Initure.Pin=MCU_T_BOX_UART_TX_PIN;			
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		          //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			            //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;		    //����

		HAL_GPIO_Init(MCU_T_BOX_UART_PART,&GPIO_Initure);//��ʼ��PA9

		GPIO_Initure.Pin=MCU_T_BOX_UART_RX_PIN;		
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	             //ģʽҪ����Ϊ��������ģʽ��
		HAL_GPIO_Init(MCU_T_BOX_UART_PART,&GPIO_Initure);	   	
		
#if 1
		HAL_NVIC_EnableIRQ(MCU_T_BOX_UART_IRQ);				   //ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(MCU_T_BOX_UART_IRQ,MCU_T_BOX_UART_IRQ_PREEMPTPRIORTY,MCU_T_BOX_UART_IRQ_SUBPRIORTY);
#endif	
	}
	
	if(huart->Instance==USART3)
	{	
		MCU_GPS_UART_GPIO_CLK();			                   //ʹ��GPIOʱ��
	  __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();		               //ʹ��USART3ʱ��
  	__HAL_AFIO_REMAP_USART3_ENABLE();
		
		
		GPIO_Initure.Pin=MCU_GPS_UART_TX_PIN;		 
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		           //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			             //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	       //����

		HAL_GPIO_Init(MCU_GPS_UART_PART,&GPIO_Initure);	   	

		GPIO_Initure.Pin=MCU_GPS_UART_RX_PIN;			 
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	             //ģʽҪ����Ϊ��������ģʽ��
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
		HAL_NVIC_EnableIRQ(MCU_GPS_UART_IRQ);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(MCU_GPS_UART_IRQ,MCU_GPS_UART_IRQ_PREEMPTPRIORTY,MCU_GPS_UART_IRQ_SUBPRIORTY);			//��ռ���ȼ�3�������ȼ�3
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
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA_M720 & 0x8000)==0)//����δ���
		{
			if(USART_RX_STA_M720&0x4000)//���յ���0x0d
			{
				if(aRxBuffer_M720[0]!=0x0a)USART_RX_STA_M720=0;//���մ���,���¿�ʼ
				else USART_RX_STA_M720|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer_M720[0]==0x0d)USART_RX_STA_M720|=0x4000;
				else
				{
					USART_RX_BUF_M720[USART_RX_STA_M720&0X3FFF]=aRxBuffer_M720[0] ;
					USART_RX_STA_M720++;
					if(USART_RX_STA_M720>(USART_REC_LEN_M720-1))USART_RX_STA_M720=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
		else
		{
			USART_RX_STA_M720=0;
		}
	}
	
	if(huart->Instance==USART2)//����Ǵ���2
	{
		if((USART_RX_STA_T_BOX & 0x8000)==0)//����δ���
		{
			if(USART_RX_STA_T_BOX&0x4000)//���յ���0x0d
			{
				if(aRxBuffer_T_BOX[0]!=0x0a)USART_RX_STA_T_BOX=0;//���մ���,���¿�ʼ
				else USART_RX_STA_T_BOX|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer_T_BOX[0]==0x0d)USART_RX_STA_T_BOX|=0x4000;
				else
				{
					USART_RX_BUF_T_BOX[USART_RX_STA_T_BOX&0X3FFF]=aRxBuffer_T_BOX[0] ;
					USART_RX_STA_T_BOX++;
					if(USART_RX_STA_T_BOX>(USART_REC_LEN_T_BOX-1))USART_RX_STA_T_BOX=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
		else
		{	
			USART_RX_STA_T_BOX=0;			
		}
	}
	
	if(huart->Instance==USART3)//����Ǵ���3
	{
		#if DMA_UART3 
		UsartReceive_IDLE(&MCU_GPS_UART3_Handler);
		#else
		
	  if((USART_RX_STA_GPS&0x8000)==0)//����δ���
		{
			if(USART_RX_STA_GPS&0x4000)//���յ���0x0d
			{
				if(aRxBuffer_GPS[0]!=0x0a)USART_RX_STA_GPS=0;//���մ���,���¿�ʼ
				else USART_RX_STA_GPS|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer_GPS[0]==0x0d)USART_RX_STA_GPS|=0x4000;
				else
				{
					USART_RX_BUF_GPS[USART_RX_STA_GPS&0X3FFF]=aRxBuffer_GPS[0] ;
					USART_RX_STA_GPS++;
					if(USART_RX_STA_GPS>(USART_REC_LEN_GPS-1))USART_RX_STA_GPS=0;//�������ݴ���,���¿�ʼ����	  
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
	
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			CLEAR_BIT(huart->ErrorCode, HAL_UART_ERROR_ORE);
		}
    
	}
	if(huart->Instance==USART2)//����Ǵ���2
	{		
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			CLEAR_BIT(huart->ErrorCode, HAL_UART_ERROR_ORE);
		}
	}
	if(huart->Instance==USART3)//����Ǵ���3
	{		
		if(uart_error_flag & HAL_UART_ERROR_ORE)
		{
			__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
			__HAL_UART_CLEAR_OREFLAG(huart);
		}	
	}
}

//���ڿ����жϴ�����
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

//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&MCU_M720_UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
  while(HAL_UART_GetState(&MCU_M720_UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	   timeout++;////��ʱ����
     if(timeout>maxDelay) break;			
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&MCU_M720_UART1_Handler, (u8 *)aRxBuffer_M720, RXBUFFERSIZE_M720) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}
} 

//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&MCU_T_BOX_UART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
  while(HAL_UART_GetState(&MCU_T_BOX_UART2_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	   timeout++;////��ʱ����
     if(timeout>maxDelay) break;	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&MCU_T_BOX_UART2_Handler, (u8 *)aRxBuffer_T_BOX, RXBUFFERSIZE_T_BOX) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}
}

//����3�жϷ������
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
  while(HAL_UART_GetState(&MCU_GPS_UART3_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	   timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
  while(HAL_UART_Receive_IT(&MCU_GPS_UART3_Handler, (u8 *)aRxBuffer_GPS, RXBUFFERSIZE_GPS) != HAL_OK)	
	{
	 timeout++; //��ʱ����
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
