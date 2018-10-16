/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    14-April-2017
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define AT_VER_REQ     "at+cgmr\r\n"          
#define AT_ICCID_REQ   "at+iccid\r\n"
#define AT_IMSI_REQ    "at+cimi\r\n"          
#define AT_SNR_REQ     "at+csq\r\n"          
#define AT_RSSI_REQ    "at+csq\r\n"
#define AT_MISS_REQ    "at\r\n"
#define AT_QUA_REQ     "at\r\n"
#define AT_TEST_REQ    "at\r\n"


uint8_t CMD_AT[20][20] = {{AT_VER_REQ},
	                        {AT_ICCID_REQ},
													{AT_IMSI_REQ},
													{AT_SNR_REQ},
													{AT_RSSI_REQ},
													{AT_MISS_REQ},
													{AT_QUA_REQ},	
                         };
uint8_t string[11] = {'1','2','3','4','5','6'};
uint8_t SPI_Receive[20] = {0};
uint8_t data[20] = {0};
uint8_t cmd[20] = {0};
uint8_t i = 0;
uint8_t canbuf_send[8] ={0,1,2,3,4,5,6,7};
uint8_t canbuf_recev[8] ={0};


int main(void)
{
	HAL_Init();
	Stm32_Clock_Init();                         //设置时钟,72Mhz
	delay_init(72);                             //初始化延时函数
	LED_Init();                                 //初始化LED  
	SPI1_Init();
	
	Uart_Init(&MCU_M720_UART1_Handler,115200);  //初始化USART
	Uart_Init(&MCU_T_BOX_UART2_Handler,115200); //初始化USART
	Uart_Init(&MCU_GPS_UART3_Handler,9600);     //初始化USART
	
	GPS_Power_IO_Init();

  TIM3_Init(10000-1,7200-1);                  //定时器3初始化，定时器时钟为72M，分频系数为7200-1，时钟10k， 转化为时间 0.0001s = 1/10k，  定时1s = 0.0001s * 10000
  
	CAN1_Mode_Init(CAN1,CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,4,CAN_MODE_LOOPBACK);
	CAN1_Mode_Init(CAN2,CAN_SJW_1TQ,CAN_BS2_8TQ,CAN_BS1_9TQ,4,CAN_MODE_LOOPBACK);
	
  while (1)
  {
		if( HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_SET)
		{
			GPS_Power_On();
			
			if(0)
			{
				CAN1_Send_Msg(canbuf_send,8);
				CAN1_Receive_Msg(canbuf_recev);				
			}

			
			if(USART_RX_STA_GPS & 0x8000)
			{
				USART_RX_STA_GPS &= (~0x8000);
				HAL_UART_Transmit_DMA(&MCU_GPS_UART3_Handler, USART_RX_BUF_GPS, USART_RX_STA_GPS);
				HAL_UART_Receive_DMA(&MCU_GPS_UART3_Handler,aRxBuffer_GPS,RXBUFFERSIZE_GPS);
			}
			
			UART_Send_messag(&MCU_T_BOX_UART2_Handler, CMD_AT[i], 10, 10);
			if(USART_RX_STA_T_BOX & 0x8000)
			{
				USART_RX_STA_T_BOX &= (~0x8000);
				UART_Send_messag(&MCU_M720_UART1_Handler,USART_RX_BUF_M720, 20, 10);
			}
			
			UART_Send_messag(&MCU_M720_UART1_Handler,CMD_AT[i], 10, 10);//CMD_AT[i]
			if(USART_RX_STA_M720 & 0x8000)
			{
				USART_RX_STA_M720 &= (~0x8000);
				UART_Send_messag(&MCU_M720_UART1_Handler,USART_RX_BUF_M720, 20, 10);
			}
//		HAL_SPI_Transmit_DMA(&SPI1_Handler, (uint8_t*)CMD_AT[i],6);
//		HAL_SPI_Receive_DMA(&SPI1_Handler, (uint8_t*)SPI_Receive,6);
//    HAL_SPI_TransmitReceive_DMA(&SPI1_Handler, (uint8_t*)CMD_AT[i], (uint8_t *)SPI_Receive, 10);
//		HAL_SPI_TransmitReceive_DMA(&SPI1_Handler, CMD_AT[i],SPI_Receive, 15);			
		}
    else
		{
			GPS_Power_Off();
		  if(HAL_SPI_Receive(&SPI1_Handler, SPI_Receive,  10, 1000) == HAL_OK)
			{
				HAL_UART_Transmit_DMA(&MCU_GPS_UART3_Handler, SPI_Receive, 10);
				HAL_Delay(500);
			}			
		}


		i++;
		if(i > 6)
		{
			i = 0;
		}
  }
}
