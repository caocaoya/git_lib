#ifndef __CAN_H
#define __CAN_H
#include "sys.h"


#define CAN1_GPIO_CLK()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define CAN1_CLK_ENABLE()          __HAL_RCC_CAN1_CLK_ENABLE()
#define CAN1_CLK_DISENABLE()       __HAL_RCC_CAN1_CLK_DISABLE()
#define CAN1_PART                  GPIOA
#define CAN1_TX_PIN                GPIO_PIN_12
#define CAN1_RX_PIN                GPIO_PIN_11
#define CAN1_R0_IRQ                USART1_IRQn
#define CAN1_R1_IRQ                USART1_IRQn
#define CAN1_R0_PREEMPTPRIORTY     4
#define CAN1_R0_SUBPRIORTY         0


#define CAN2_GPIO_CLK()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define CAN2_CLK_ENABLE()          __HAL_RCC_CAN2_CLK_ENABLE()
#define CAN2_CLK_DISENABLE()       __HAL_RCC_CAN2_CLK_DISABLE()
#define CAN2_PART                  GPIOB
#define CAN2_TX_PIN                GPIO_PIN_13
#define CAN2_RX_PIN                GPIO_PIN_12
#define CAN2_R0_IRQ                USART1_IRQn
#define CAN2_R1_IRQ                USART1_IRQn
#define CAN2_R0_PREEMPTPRIORTY     4
#define CAN2_R0_SUBPRIORTY         0


//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.

u8 CAN1_Mode_Init(CAN_TypeDef *canx, u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN初始化
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据
#endif
