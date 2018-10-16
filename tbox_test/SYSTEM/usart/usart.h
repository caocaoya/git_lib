#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

#define MCU_M720_UART_GPIO_CLK()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCU_M720_UART_PART                  GPIOA
#define MCU_M720_UART_TX_PIN                GPIO_PIN_9
#define MCU_M720_UART_RX_PIN                GPIO_PIN_10
#define MCU_M720_UART_IRQ                   USART1_IRQn
#define MCU_M720_UART_IRQ_PREEMPTPRIORTY    4
#define MCU_M720_UART_IRQ_SUBPRIORTY        0

#define MCU_T_BOX_UART_GPIO_CLK()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCU_T_BOX_UART_PART                  GPIOA
#define MCU_T_BOX_UART_TX_PIN                GPIO_PIN_2
#define MCU_T_BOX_UART_RX_PIN                GPIO_PIN_3
#define MCU_T_BOX_UART_IRQ                   USART2_IRQn
#define MCU_T_BOX_UART_IRQ_PREEMPTPRIORTY    3
#define MCU_T_BOX_UART_IRQ_SUBPRIORTY        0

#define MCU_GPS_UART_GPIO_CLK()              __HAL_RCC_GPIOD_CLK_ENABLE()
#define MCU_GPS_UART_PART                    GPIOD
#define MCU_GPS_UART_TX_PIN                  GPIO_PIN_8
#define MCU_GPS_UART_RX_PIN                  GPIO_PIN_9
#define MCU_GPS_UART_IRQ                     USART3_IRQn
#define MCU_GPS_UART_IRQ_PREEMPTPRIORTY      4
#define MCU_GPS_UART_IRQ_SUBPRIORTY          0

/* Definition for SPIx's DMA */
#define UART3_TX_DMA_CHANNEL              DMA1_Channel2
#define UART3_RX_DMA_CHANNEL              DMA1_Channel3

/* Definition for SPIx's NVIC */
#define UART3_DMA_TX_IRQn                 DMA1_Channel2_IRQn
#define UART3_DMA_RX_IRQn                 DMA1_Channel3_IRQn
#define UART3_DMA_TX_IRQHandler           DMA1_Channel2_IRQHandler
#define UART3_DMA_RX_IRQHandler           DMA1_Channel3_IRQHandler



#define USART_REC_LEN_M720  			50  	//定义最大接收字节数 200
#define USART_REC_LEN_T_BOX  			50  	//定义最大接收字节数 200
#define USART_REC_LEN_GPS 		  	50  	//定义最大接收字节数 200

#define RXBUFFERSIZE_M720         1 //缓存大小
#define RXBUFFERSIZE_T_BOX        1 //缓存大小
#define RXBUFFERSIZE_GPS          128 //缓存大小

#define MCU_M720_TX_TIME     10
#define MCU_M720_RX_TIME     2000
	  

		
extern u8  USART_RX_BUF_M720[USART_REC_LEN_M720];   //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA_M720;                       //接收状态标记	
extern u8  USART_RX_BUF_GPS[USART_REC_LEN_GPS];     //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA_GPS;         		            //接收状态标记	
extern u8  USART_RX_BUF_T_BOX[USART_REC_LEN_T_BOX]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA_T_BOX;         		          //接收状态标记	

extern UART_HandleTypeDef  MCU_M720_UART1_Handler;   //UART句柄
extern UART_HandleTypeDef  MCU_T_BOX_UART2_Handler;  //UART句柄
extern UART_HandleTypeDef  MCU_GPS_UART3_Handler;    //UART句柄

extern u8 aRxBuffer_M720[RXBUFFERSIZE_M720];//HAL库使用的串口接收缓冲
extern u8 aRxBuffer_GPS[RXBUFFERSIZE_GPS];//HAL库使用的串口接收缓冲
extern u8 aRxBuffer_T_BOX[RXBUFFERSIZE_T_BOX];//HAL库使用的串口接收缓冲


//如果想串口中断接收，请不要注释以下宏定义
void Uart_Init(UART_HandleTypeDef *Uart_Handler, u32 bound);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);
HAL_StatusTypeDef UART_Send_messag(UART_HandleTypeDef *huart, uint8_t *p_string, uint8_t size, uint32_t Timeout);
void UART_Recevie_messag(UART_HandleTypeDef *huart, uint8_t *p_string, uint16_t size, uint32_t Timeout);
void M720_Info_Req(uint8_t *type, uint8_t *key, uint16_t size);

#endif
