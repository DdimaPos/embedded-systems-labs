#pragma once

#define TEMP_SENSOR_PIN 4

void  TempSensor_Init(void);
float TempSensor_Read(void);
int   TempSensor_IsValid(void);
