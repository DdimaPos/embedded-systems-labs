#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

#define LED_PIN LED_BUILTIN

void setupLed();

void ledOn();
void ledOff();
void ledSOS();

#endif
