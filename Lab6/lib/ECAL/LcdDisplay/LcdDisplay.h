#pragma once
#include <stdint.h>

void LcdDisplay_Init(void);
void LcdDisplay_Clear(void);
void LcdDisplay_SetCursor(uint8_t col, uint8_t row);
void LcdDisplay_Print(const char *str);
void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *str);
