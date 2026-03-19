#include "SerialIO.h"
#include <Arduino.h>

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    delay(2000);
}
