#pragma once

/* --- Wiring --- */
#define LED_GREEN_PIN  25
#define LED_RED_PIN    26

void LedDriver_Init(void);
void LedDriver_GreenOn(void);
void LedDriver_GreenOff(void);
void LedDriver_RedOn(void);
void LedDriver_RedOff(void);
