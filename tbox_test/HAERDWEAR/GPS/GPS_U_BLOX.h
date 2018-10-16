#ifndef _GPS_U_BLOX_H
#define _GPS_U_BLOX_H
#include "sys.h"

#define GPS_POWER_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define GPS_POWER_GPIO_PART           GPIOD
#define GPS_POWER_GPIO_PIN            GPIO_PIN_0



void GPS_Power_IO_Init(void);
void GPS_Power_On(void);
void GPS_Power_Off(void);
uint8_t GPS_Init(void);
void GPS_Read_Data(void);

#endif
