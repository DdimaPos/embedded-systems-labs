#include "LdrDriver.h"
#include <Arduino.h>

void LdrDriver_Init(void)
{
    /* GPIO34 is input-only; no pinMode call needed for ADC use.
     * Explicitly set 12-bit resolution to guarantee the 0-4095 range
     * regardless of Arduino-ESP32 framework default. */
    analogReadResolution(LDR_ADC_BITS);

    /* Warm-up read: the first ADC sample after a long idle period can be
     * slightly off due to capacitor charge-up; discard it. */
    (void)analogRead(LDR_PIN);
}

int LdrDriver_ReadRaw(void)
{
    return analogRead(LDR_PIN);
}

float LdrDriver_ReadPercent(void)
{
    float pct = ((float)analogRead(LDR_PIN) / LDR_ADC_MAX) * 100.0f;
    if (pct < 0.0f)   pct = 0.0f;
    if (pct > 100.0f) pct = 100.0f;
    return pct;
}
