#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

#define LED_GREEN_PIN LED_BUILTIN
#define LED_RED_PIN 12
#define LED_YELLOW_PIN 11

void setupGreenLed();
void setupRedLed();
void setupYellowLed();

void ledGreenOn();
void ledGreenOff();
bool ledGreenIsOn();

void ledRedOn();
void ledRedOff();

void ledYellowOn();
void ledYellowOff();

#endif
