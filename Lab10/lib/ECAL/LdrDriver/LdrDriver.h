#pragma once
#include <stdint.h>

#define LDR_PIN         34       /* GPIO34 = ADC1_CH6, input-only pin        */
#define LDR_ADC_BITS    12       /* 12-bit resolution: 0 – 4095              */
#define LDR_ADC_MAX     4095.0f

/*
 * Voltage divider: VCC → LDR → GPIO34 → 10 kΩ → GND
 * Higher ambient light → lower LDR resistance → higher voltage → higher ADC value
 * Therefore: 100 % = maximum light, 0 % = darkness
 */

void  LdrDriver_Init(void);
float LdrDriver_ReadPercent(void);  /* Normalised light level: 0.0 – 100.0 % */
int   LdrDriver_ReadRaw(void);      /* Raw ADC value: 0 – 4095 (diagnostics)  */
