#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
extern TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 
extern TIM_HandleTypeDef TIM5_Handler; 
extern TIM_OC_InitTypeDef TIM3_CH4Handler;	    //��ʱ��3ͨ��4���

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
void TIM_SetTIM3Compare4(u32 compare);
u32 TIM_GetTIM3Capture4(void);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
#endif

