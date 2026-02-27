#include "button.h"
#include <Arduino.h>

void buttonSetup() { pinMode(BUTTON_PIN, INPUT_PULLUP); }

bool buttonIsPressed() { return digitalRead(BUTTON_PIN) == LOW; }
