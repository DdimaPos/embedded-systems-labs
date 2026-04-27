#pragma once

#define SERIAL_BAUD 115200

void SerialIoInit(void);

/* Non-blocking character read.
 * Returns the next available byte (0–255) or -1 if no data is waiting.
 * Serial.available()/Serial.read() are encapsulated here so that no
 * higher layer ever needs to call Serial.* directly. */
int  SerialIO_ReadChar(void);
