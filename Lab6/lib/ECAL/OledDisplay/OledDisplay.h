#pragma once
#include <stdint.h>

#define OLED_WIDTH      128
#define OLED_HEIGHT      64
#define OLED_I2C_ADDR  0x3C

void OledDisplay_Init(void);
void OledDisplay_Clear(void);
void OledDisplay_SetCursor(uint8_t x, uint8_t y);
void OledDisplay_SetTextSize(uint8_t size);
void OledDisplay_Print(const char *str);
void OledDisplay_PrintAt(uint8_t x, uint8_t y, const char *str);
void OledDisplay_Update(void);
