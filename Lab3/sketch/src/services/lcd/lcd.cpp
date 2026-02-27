#include "lcd.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <stdio.h>

#define CLEAR_KEY '*'

// If 0x27 doesn't work, try 0x3F!
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int srvLcdPutChar(char c, FILE *stream) {
  lcd.print(c);
  return c;
}

void srvLcdClearChar() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

void srvLcdSetup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);

  FILE *myStream = fdevopen(srvLcdPutChar, NULL);
  stdout = myStream;
}
