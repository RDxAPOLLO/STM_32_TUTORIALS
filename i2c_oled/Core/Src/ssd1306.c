#include "ssd1306.h"


/* hi2c1 is created by CubeMX in main.c */
extern I2C_HandleTypeDef hi2c1;


/* 128 x 64 / 8 = 1024 bytes */
static uint8_t SSD1306_Buffer[1024];


/* Current text position */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;


/* ----------------------------------------------------------
   Send one command to SSD1306
   ---------------------------------------------------------- */

static void SSD1306_WriteCommand(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;       // Control byte: command
    data[1] = command;

    HAL_I2C_Master_Transmit(
        &hi2c1,
        SSD1306_ADDR,
        data,
        2,
        HAL_MAX_DELAY
    );
}


/* ----------------------------------------------------------
   Send data to SSD1306
   ---------------------------------------------------------- */

static void SSD1306_WriteData(uint8_t *data, uint16_t size)
{
    uint8_t buffer[129];

    buffer[0] = 0x40;     // Control byte: data

    for (uint16_t i = 0; i < size; i++)
    {
        buffer[i + 1] = data[i];
    }

    HAL_I2C_Master_Transmit(
        &hi2c1,
        SSD1306_ADDR,
        buffer,
        size + 1,
        HAL_MAX_DELAY
    );
}


/* ----------------------------------------------------------
   SSD1306 Initialization
   ---------------------------------------------------------- */

void SSD1306_Init(void)
{
    HAL_Delay(100);


    /* Display OFF */
    SSD1306_WriteCommand(0xAE);


    /* Set memory addressing mode */
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x00);


    /* Set page start address */
    SSD1306_WriteCommand(0xB0);


    /* Set COM output scan direction */
    SSD1306_WriteCommand(0xC8);


    /* Set low column address */
    SSD1306_WriteCommand(0x00);


    /* Set high column address */
    SSD1306_WriteCommand(0x10);


    /* Set display start line */
    SSD1306_WriteCommand(0x40);


    /* Set contrast */
    SSD1306_WriteCommand(0x81);
    SSD1306_WriteCommand(0x7F);


    /* Segment remap */
    SSD1306_WriteCommand(0xA1);


    /* Normal display */
    SSD1306_WriteCommand(0xA6);


    /* Set multiplex ratio */
    SSD1306_WriteCommand(0xA8);
    SSD1306_WriteCommand(0x3F);


    /* Display follows RAM */
    SSD1306_WriteCommand(0xA4);


    /* Display offset */
    SSD1306_WriteCommand(0xD3);
    SSD1306_WriteCommand(0x00);


    /* Display clock divide ratio */
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(0x80);


    /* Pre-charge period */
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(0xF1);


    /* COM pins hardware configuration */
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(0x12);


    /* VCOMH deselect level */
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(0x40);


    /* Charge pump */
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(0x14);


    /* Clear display RAM */
    SSD1306_Clear();


    /* Send cleared buffer to OLED */
    SSD1306_UpdateScreen();


    /* Display ON */
    SSD1306_WriteCommand(0xAF);
}


/* ----------------------------------------------------------
   Clear framebuffer
   ---------------------------------------------------------- */

void SSD1306_Clear(void)
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        SSD1306_Buffer[i] = 0x00;
    }
}


/* ----------------------------------------------------------
   Update OLED from framebuffer
   ---------------------------------------------------------- */

void SSD1306_UpdateScreen(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        /* Select page */
        SSD1306_WriteCommand(0xB0 + page);


        /* Set column address */
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);


        /* Send 128 bytes */
        SSD1306_WriteData(
            &SSD1306_Buffer[page * 128],
            128
        );
    }
}


/* ----------------------------------------------------------
   Draw one pixel
   ---------------------------------------------------------- */

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
        return;


    /*
       Each byte represents 8 vertical pixels.

       Page = y / 8
       Bit  = y % 8
    */

    uint16_t index = x + (y / 8) * 128;

    uint8_t bit = 1 << (y % 8);


    if (color == SSD1306_COLOR_WHITE)
    {
        SSD1306_Buffer[index] |= bit;
    }
    else
    {
        SSD1306_Buffer[index] &= ~bit;
    }
}


/* ----------------------------------------------------------
   Set text cursor
   ---------------------------------------------------------- */

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{
    cursor_x = x;
    cursor_y = y;
}


/* ----------------------------------------------------------
   5x7 Font
   ---------------------------------------------------------- */

static const uint8_t Font5x7[][5] =
{
    /* SPACE */
    {0x00,0x00,0x00,0x00,0x00},

    /* ! */
    {0x00,0x00,0x5F,0x00,0x00},

    /* " */
    {0x00,0x07,0x00,0x07,0x00},

    /* # */
    {0x14,0x7F,0x14,0x7F,0x14},

    /* $ */
    {0x24,0x2A,0x7F,0x2A,0x12},

    /* % */
    {0x23,0x13,0x08,0x64,0x62},

    /* & */
    {0x36,0x49,0x55,0x22,0x50},

    /* ' */
    {0x00,0x05,0x03,0x00,0x00},

    /* ( */
    {0x00,0x1C,0x22,0x41,0x00},

    /* ) */
    {0x00,0x41,0x22,0x1C,0x00},

    /* * */
    {0x14,0x08,0x3E,0x08,0x14},

    /* + */
    {0x08,0x08,0x3E,0x08,0x08},

    /* , */
    {0x00,0x50,0x30,0x00,0x00},

    /* - */
    {0x08,0x08,0x08,0x08,0x08},

    /* . */
    {0x00,0x60,0x60,0x00,0x00},

    /* / */
    {0x20,0x10,0x08,0x04,0x02},

    /* 0 */
    {0x3E,0x51,0x49,0x45,0x3E},

    /* 1 */
    {0x00,0x42,0x7F,0x40,0x00},

    /* 2 */
    {0x42,0x61,0x51,0x49,0x46},

    /* 3 */
    {0x21,0x41,0x45,0x4B,0x31},

    /* 4 */
    {0x18,0x14,0x12,0x7F,0x10},

    /* 5 */
    {0x27,0x45,0x45,0x45,0x39},

    /* 6 */
    {0x3C,0x4A,0x49,0x49,0x30},

    /* 7 */
    {0x01,0x71,0x09,0x05,0x03},

    /* 8 */
    {0x36,0x49,0x49,0x49,0x36},

    /* 9 */
    {0x06,0x49,0x49,0x29,0x1E},

    /* : */
    {0x00,0x36,0x36,0x00,0x00},

    /* ; */
    {0x00,0x56,0x36,0x00,0x00},

    /* < */
    {0x08,0x14,0x22,0x41,0x00},

    /* = */
    {0x14,0x14,0x14,0x14,0x14},

    /* > */
    {0x00,0x41,0x22,0x14,0x08},

    /* ? */
    {0x02,0x01,0x51,0x09,0x06},

    /* @ */
    {0x32,0x49,0x79,0x41,0x3E},

    /* A */
    {0x7E,0x11,0x11,0x11,0x7E},

    /* B */
    {0x7F,0x49,0x49,0x49,0x36},

    /* C */
    {0x3E,0x41,0x41,0x41,0x22},

    /* D */
    {0x7F,0x41,0x41,0x22,0x1C},

    /* E */
    {0x7F,0x49,0x49,0x49,0x41},

    /* F */
    {0x7F,0x09,0x09,0x09,0x01},

    /* G */
    {0x3E,0x41,0x49,0x49,0x7A},

    /* H */
    {0x7F,0x08,0x08,0x08,0x7F},

    /* I */
    {0x00,0x41,0x7F,0x41,0x00},

    /* J */
    {0x20,0x40,0x41,0x3F,0x01},

    /* K */
    {0x7F,0x08,0x14,0x22,0x41},

    /* L */
    {0x7F,0x40,0x40,0x40,0x40},

    /* M */
    {0x7F,0x02,0x0C,0x02,0x7F},

    /* N */
    {0x7F,0x04,0x08,0x10,0x7F},

    /* O */
    {0x3E,0x41,0x41,0x41,0x3E},

    /* P */
    {0x7F,0x09,0x09,0x09,0x06},

    /* Q */
    {0x3E,0x41,0x51,0x21,0x5E},

    /* R */
    {0x7F,0x09,0x19,0x29,0x46},

    /* S */
    {0x46,0x49,0x49,0x49,0x31},

    /* T */
    {0x01,0x01,0x7F,0x01,0x01},

    /* U */
    {0x3F,0x40,0x40,0x40,0x3F},

    /* V */
    {0x1F,0x20,0x40,0x20,0x1F},

    /* W */
    {0x3F,0x40,0x38,0x40,0x3F},

    /* X */
    {0x63,0x14,0x08,0x14,0x63},

    /* Y */
    {0x07,0x08,0x70,0x08,0x07},

    /* Z */
    {0x61,0x51,0x49,0x45,0x43}
};


/* ----------------------------------------------------------
   Write one character
   ---------------------------------------------------------- */

void SSD1306_WriteChar(char c)
{
    /*
       Font supports ASCII 32 to 90
    */

    if (c < 32 || c > 90)
    {
        c = '?';
    }


    uint8_t index = c - 32;


    /* Draw 5 columns */
    for (uint8_t column = 0; column < 5; column++)
    {
        uint8_t line = Font5x7[index][column];


        for (uint8_t row = 0; row < 7; row++)
        {
            if (line & (1 << row))
            {
                SSD1306_DrawPixel(
                    cursor_x + column,
                    cursor_y + row,
                    SSD1306_COLOR_WHITE
                );
            }
            else
            {
                SSD1306_DrawPixel(
                    cursor_x + column,
                    cursor_y + row,
                    SSD1306_COLOR_BLACK
                );
            }
        }
    }


    /* One blank column between characters */
    cursor_x += 6;


    /* Move to next line */
    if (cursor_x + 5 >= SSD1306_WIDTH)
    {
        cursor_x = 0;
        cursor_y += 8;
    }
}


/* ----------------------------------------------------------
   Write string
   ---------------------------------------------------------- */

void SSD1306_WriteString(char *str)
{
    while (*str)
    {
        SSD1306_WriteChar(*str);
        str++;
    }
}
