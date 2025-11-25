#ifndef ST7789_H
#define ST7789_H

#include "main.h"

// -------------------- Display Resolution --------------------
#define ST7789_WIDTH   240
#define ST7789_HEIGHT  240

// -------------------- SPI & GPIO bindings --------------------
// ????? ?????????????? ???????? ???? GPIO!

#define ST7789_SPI      SPI1

#define ST7789_DC_GPIO  GPIOA
#define ST7789_DC_PIN   LL_GPIO_PIN_1

#define ST7789_CS_GPIO  GPIOA
#define ST7789_CS_PIN   LL_GPIO_PIN_2

#define ST7789_RST_GPIO GPIOA
#define ST7789_RST_PIN  LL_GPIO_PIN_3

// -------------------- API --------------------
void ST7789_Init(void);
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7789_FillScreen(uint16_t color);

void ST7789_DrawChar(int x, int y, char c, uint16_t color, uint16_t bg, uint8_t size);
void ST7789_DrawString(int x, int y, const char *str, uint16_t color, uint16_t bg, uint8_t size);

#endif