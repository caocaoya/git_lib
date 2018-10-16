#include "led.h"


//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4; //PB1,0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1 
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1  
}
