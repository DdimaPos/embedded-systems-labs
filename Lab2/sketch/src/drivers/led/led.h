#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

#define LED_PIN LED_BUILTIN

void setupGreenLed();

void setupRedLed();

void ledGreenOn();
void ledGreenOff();
void ledRedOn();
void ledRedOff();

void ledSOS();

#endif
