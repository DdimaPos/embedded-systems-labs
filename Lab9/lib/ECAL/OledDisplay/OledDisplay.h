#pragma once
#include <stdint.h>

/* --- Wiring (I2C, shared bus with LCD) --- */
#define OLED_WIDTH      128
#define OLED_HEIGHT      64
#define OLED_I2C_ADDR  0x3C
#define OLED_SDA_PIN    21   /* ESP32 default I2C SDA */
#define OLED_SCL_PIN    22   /* ESP32 default I2C SCL */

void OledDisplay_Init(void);
void OledDisplay_Clear(void);
void OledDisplay_SetCursor(uint8_t x, uint8_t y);
void OledDisplay_SetTextSize(uint8_t size);
void OledDisplay_Print(const char *str);
void OledDisplay_PrintAt(uint8_t x, uint8_t y, const char *str);
void OledDisplay_Update(void);
