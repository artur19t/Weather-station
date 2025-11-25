#include "st7789.h"
#include "font8x8.h"
#include <string.h>

// -------------------- Low-level SPI helpers --------------------
static inline void ST7789_Select(void) {
  LL_GPIO_ResetOutputPin(ST7789_CS_GPIO, ST7789_CS_PIN);
}
static inline void ST7789_Unselect(void) {
  LL_GPIO_SetOutputPin(ST7789_CS_GPIO, ST7789_CS_PIN);
}
static inline void ST7789_DC_Command(void) {
  LL_GPIO_ResetOutputPin(ST7789_DC_GPIO, ST7789_DC_PIN);
}
static inline void ST7789_DC_Data(void) {
  LL_GPIO_SetOutputPin(ST7789_DC_GPIO, ST7789_DC_PIN);
}
static inline void ST7789_ResetPulse(void) {
  LL_GPIO_ResetOutputPin(ST7789_RST_GPIO, ST7789_RST_PIN);
  for (volatile int i = 0; i < 20000; i++);
  LL_GPIO_SetOutputPin(ST7789_RST_GPIO, ST7789_RST_PIN);
  for (volatile int i = 0; i < 20000; i++);
}

static void ST7789_WriteCommand(uint8_t cmd)
{
  ST7789_DC_Command();
  ST7789_Select();
  while (!LL_SPI_IsActiveFlag_TXE(ST7789_SPI)) {}
  LL_SPI_TransmitData8(ST7789_SPI, cmd);
  while (LL_SPI_IsActiveFlag_BSY(ST7789_SPI)) {}
  ST7789_Unselect();
}

void ST7789_WriteData(uint8_t data)
{
  ST7789_DC_Data();
  ST7789_Select();
  while (!LL_SPI_IsActiveFlag_TXE(ST7789_SPI)) {}
  LL_SPI_TransmitData8(ST7789_SPI, data);
  while (LL_SPI_IsActiveFlag_BSY(ST7789_SPI)) {}
  ST7789_Unselect();
}

void ST7789_WriteData16(uint16_t data)
{
  ST7789_DC_Data();
  ST7789_Select();
  while (!LL_SPI_IsActiveFlag_TXE(ST7789_SPI));
  LL_SPI_TransmitData8(ST7789_SPI, data >> 8);
  while (!LL_SPI_IsActiveFlag_TXE(ST7789_SPI));
  LL_SPI_TransmitData8(ST7789_SPI, data & 0xFF);
  while (LL_SPI_IsActiveFlag_BSY(ST7789_SPI));
  ST7789_Unselect();
}

// -------------------- Address Window --------------------
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  ST7789_WriteCommand(0x2A);
  ST7789_WriteData(x0 >> 8);
  ST7789_WriteData(x0 & 0xFF);
  ST7789_WriteData(x1 >> 8);
  ST7789_WriteData(x1 & 0xFF);

  ST7789_WriteCommand(0x2B);
  ST7789_WriteData(y0 >> 8);
  ST7789_WriteData(y0 & 0xFF);
  ST7789_WriteData(y1 >> 8);
  ST7789_WriteData(y1 & 0xFF);

  ST7789_WriteCommand(0x2C);
}

// -------------------- Basic Drawing --------------------
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  ST7789_SetAddressWindow(x, y, x, y);
  ST7789_WriteData16(color);
}

void ST7789_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);
  for (uint32_t i = 0; i < (uint32_t)w * h; i++)
  {
    ST7789_WriteData16(color);
  }
}

void ST7789_FillScreen(uint16_t color)
{
  ST7789_FillRect(0, 0, ST7789_WIDTH, ST7789_HEIGHT, color);
}

// -------------------- Init sequence for ST7789 --------------------
void ST7789_Init(void)
{
  ST7789_ResetPulse();

  ST7789_WriteCommand(0x11); // Sleep Out
  for (volatile int i = 0; i < 200000; i++);

  ST7789_WriteCommand(0x3A); // COLMOD
  ST7789_WriteData(0x55);    // 16-bit color

  ST7789_WriteCommand(0x36); // MADCTL (rotation)
  ST7789_WriteData(0x00);

  ST7789_WriteCommand(0x21); // Inversion ON

  ST7789_WriteCommand(0x29); // Display ON

  ST7789_FillScreen(0x0000);
}

// -------------------- Text Drawing --------------------
void ST7789_DrawChar(int x, int y, char c, uint16_t color, uint16_t bg, uint8_t size)
{
  if (c < 32 || c > 126) c = '?';
  const uint8_t *bitmap = font8x8[c - 32];

  for (int row = 0; row < 8; row++)
  {
    uint8_t line = bitmap[row];
    for (int col = 0; col < 8; col++)
    {
      uint16_t pixelColor = (line & (1 << (7 - col))) ? color : bg;

      if (size == 1)
      {
        ST7789_DrawPixel(x + col, y + row, pixelColor);
      }
      else
      {
        ST7789_FillRect(x + col * size, y + row * size, size, size, pixelColor);
      }
    }
  }
}

void ST7789_DrawString(int x, int y, const char *str, uint16_t color, uint16_t bg, uint8_t size)
{
  while (*str)
  {
    ST7789_DrawChar(x, y, *str, color, bg, size);
    x += 8 * size;
    str++;
  }
}
