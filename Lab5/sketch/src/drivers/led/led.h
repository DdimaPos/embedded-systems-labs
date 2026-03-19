#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

#define LED_GREEN_PIN 13
#define LED_RED_PIN 26
#define LED_YELLOW_PIN 32

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
