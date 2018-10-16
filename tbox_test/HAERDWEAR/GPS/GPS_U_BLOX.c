

#include "usart.h"
#include "gps_u_blox.h"
#include "delay.h"

void GPS_Power_IO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
  GPS_POWER_GPIO_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = GPS_POWER_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
  HAL_GPIO_Init(GPS_POWER_GPIO_PART, &GPIO_InitStruct);

	GPS_Power_On();
}
void GPS_Power_On()
{
	HAL_GPIO_WritePin(GPS_POWER_GPIO_PART, GPS_POWER_GPIO_PIN, GPIO_PIN_SET);
}
void GPS_Power_Off()
{
	HAL_GPIO_WritePin(GPS_POWER_GPIO_PART, GPS_POWER_GPIO_PIN, GPIO_PIN_RESET);
}

uint8_t GPS_Init()
{
	return 1;
}

void GPS_Read_Data()
{
	
}
