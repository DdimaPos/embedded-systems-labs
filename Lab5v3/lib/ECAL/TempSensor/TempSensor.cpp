#include "TempSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire           oneWire(TEMP_SENSOR_PIN);
static DallasTemperature sensors(&oneWire);
static float             lastTemp = 20.0f;
static int               lastValid = 0;

void TempSensor_Init(void)
{
    sensors.begin();
    sensors.setResolution(9);             // 9-bit: ~94ms conversion
    sensors.setWaitForConversion(false);  // non-blocking
    sensors.requestTemperatures();        // start first conversion
    delay(100);
}

float TempSensor_Read(void)
{
    // Non-blocking: get previous result, request next
    float temp = sensors.getTempCByIndex(0);
    lastValid = (temp != DEVICE_DISCONNECTED_C) ? 1 : 0;
    if (lastValid) {
        lastTemp = temp;
    }
    sensors.requestTemperatures();  // queue next read
    return lastTemp;
}

int TempSensor_IsValid(void)
{
    return lastValid;
}
