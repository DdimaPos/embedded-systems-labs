#include "LcdDisplay.h"
#include <LiquidCrystal_I2C.h>

// I2C address 0x27 (or 0x3F); cols=16, rows=2
// For ESP32: SDA=21, SCL=22
static LiquidCrystal_I2C lcd(0x27, 20, 4);

void LcdDisplay_Init(void) {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void LcdDisplay_Clear(void) { lcd.clear(); }

void LcdDisplay_SetCursor(uint8_t col, uint8_t row) { lcd.setCursor(col, row); }

void LcdDisplay_Print(const char *str) { lcd.print(str); }

void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *str) {
  lcd.setCursor(col, row);
  lcd.print(str);
}
