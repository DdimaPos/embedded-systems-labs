#include "TempSensor.h"
#include <DHT.h>

static DHT dht(TEMP_SENSOR_PIN, DHT11);
static float lastTemp  = 20.0f;
static int   lastValid = 0;

void TempSensor_Init(void)
{
    dht.begin();
    delay(1000);  // DHT11 needs ~1s startup
}

float TempSensor_Read(void)
{
    float temp = dht.readTemperature();
    lastValid = !isnan(temp);
    if (lastValid) {
        lastTemp = temp;
    }
    return lastTemp;
}

int TempSensor_IsValid(void)
{
    return lastValid;
}
