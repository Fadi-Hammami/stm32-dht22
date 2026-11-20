#ifndef INC_DHT22_H_
#define INC_DHT22_H_

#include "main.h"

void delay_us(uint16_t us);

void DHT22_Start(void);
uint8_t DHT22_Check_Response(void);
uint8_t DHT22_Read(void);

float DHT22_GetTemperature(void);

#endif
