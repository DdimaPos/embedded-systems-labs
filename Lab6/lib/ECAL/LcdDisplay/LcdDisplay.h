#pragma once
#include <stdint.h>

/* --- Wiring (I2C, shared bus with OLED) --- */
#define LCD_I2C_ADDR   0x27
#define LCD_COLS       20
#define LCD_ROWS        4
#define LCD_SDA_PIN    21   /* ESP32 default I2C SDA */
#define LCD_SCL_PIN    22   /* ESP32 default I2C SCL */

void LcdDisplay_Init(void);
void LcdDisplay_Clear(void);
void LcdDisplay_SetCursor(uint8_t col, uint8_t row);
void LcdDisplay_Print(const char *str);
void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *str);
