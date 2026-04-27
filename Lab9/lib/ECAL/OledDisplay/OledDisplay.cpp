#include "OledDisplay.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void OledDisplay_Init(void)
{
    oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(1);
    oled.display();
}

void OledDisplay_Clear(void)
{
    oled.clearDisplay();
}

void OledDisplay_SetCursor(uint8_t x, uint8_t y)
{
    oled.setCursor(x, y);
}

void OledDisplay_SetTextSize(uint8_t size)
{
    oled.setTextSize(size);
}

void OledDisplay_Print(const char *str)
{
    oled.print(str);
}

void OledDisplay_PrintAt(uint8_t x, uint8_t y, const char *str)
{
    oled.setCursor(x, y);
    oled.print(str);
}

void OledDisplay_Update(void)
{
    oled.display();
}
