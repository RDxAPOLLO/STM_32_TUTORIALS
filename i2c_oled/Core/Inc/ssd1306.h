#ifndef SSD1306_H
#define SSD1306_H

#include "main.h"
#include <stdint.h>

#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64
#define SSD1306_ADDR        (0x3C << 1)

#define SSD1306_COLOR_BLACK 0
#define SSD1306_COLOR_WHITE 1

void SSD1306_Init(void);

void SSD1306_Clear(void);
void SSD1306_UpdateScreen(void);

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

void SSD1306_SetCursor(uint8_t x, uint8_t y);

void SSD1306_WriteChar(char c);
void SSD1306_WriteString(char *str);

#endif
