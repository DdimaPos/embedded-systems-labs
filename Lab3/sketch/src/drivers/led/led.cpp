#include "led.h"
#include <Arduino.h>

int ledPinRed;
int ledPinGreen;
int ledPinYellow;

void setupGreenLed() {
  ledPinGreen = LED_GREEN_PIN;
  pinMode(ledPinGreen, OUTPUT);
}

void setupRedLed() {
  ledPinRed = LED_RED_PIN;
  pinMode(ledPinRed, OUTPUT);
}

void setupYellowLed() {
  ledPinYellow = LED_YELLOW_PIN;
  pinMode(ledPinYellow, OUTPUT);
}

void ledGreenOn() { digitalWrite(ledPinGreen, HIGH); }
void ledGreenOff() { digitalWrite(ledPinGreen, LOW); }

bool ledGreenIsOn() { return digitalRead(ledPinGreen) == HIGH; }

void ledRedOn() { digitalWrite(ledPinRed, HIGH); }
void ledRedOff() { digitalWrite(ledPinRed, LOW); }

void ledYellowOn() { digitalWrite(ledPinYellow, HIGH); }
void ledYellowOff() { digitalWrite(ledPinYellow, LOW); }
