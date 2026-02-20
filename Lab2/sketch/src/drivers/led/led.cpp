#include "led.h"
#include <Arduino.h>

int ledPinRed;
int ledPinGreen;

void setupGreenLed() {
  ledPinRed = 11;
  pinMode(ledPinRed, OUTPUT);
};

void setupRedLed() {
  ledPinGreen = 10;
  pinMode(ledPinGreen, OUTPUT);
};

void ledGreenOn() { digitalWrite(ledPinGreen, HIGH); }
void ledGreenOff() { digitalWrite(ledPinGreen, LOW); }
void ledRedOn() { digitalWrite(ledPinRed, HIGH); }
void ledRedOff() { digitalWrite(ledPinRed, LOW); }

void ledSOS() {
  // s
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(200);

  // o
  digitalWrite(ledPinRed, HIGH);
  delay(300);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(300);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(300);
  digitalWrite(ledPinRed, LOW);
  delay(200);

  // s
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(100);
  digitalWrite(ledPinRed, HIGH);
  delay(150);
  digitalWrite(ledPinRed, LOW);
  delay(200);
}
