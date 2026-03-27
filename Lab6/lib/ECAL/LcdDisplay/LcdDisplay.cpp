#include "LcdDisplay.h"
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

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
