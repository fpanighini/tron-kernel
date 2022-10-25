#include <videoDriver.h>
#include <font.h>
#include <lib.h>

Color black = {0x00, 0x00, 0x00};
Color white = {0xFF, 0xFF, 0xFF};
Color green = {0x1F, 0xED, 0x11};
Color gray = {0x90, 0x90, 0x90};
Color blue = {0xFF, 0x00, 0x00};
Color red = {0x00, 0x00, 0xFF};

// OSDev
struct vbe_mode_info_structure {
    uint16_t attributes;        // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;           // deprecated
    uint8_t window_b;           // deprecated
    uint16_t granularity;       // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;      // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;         // number of bytes per horizontal line
    uint16_t width;         // width in pixels
    uint16_t height;            // height in pixels
    uint8_t w_char;         // unused...
    uint8_t y_char;         // ...
    uint8_t planes;
    uint8_t bpp;            // bits per pixel in this mode
    uint8_t banks;          // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;      // deprecated; fontSize of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;       // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;   // fontSize of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed));

uint32_t fontSize = 1;

static uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);

static uint8_t buffer[64] = {'0'};

static struct vbe_mode_info_structure *screenData = (void *)0x5C00;

void printSquare(uint16_t x, uint16_t y, uint16_t l, Color color)
{
    fillrect(x, y, l, l, color);
}

void printChar(uint16_t x, uint16_t y, uint8_t c, Color color)
{
    if (c < FIRST_CHAR || c > LAST_CHAR)
    {
        return;
    }
    int charId = c - FIRST_CHAR;
    char l;
    int count = 0;

    for (int i = 0; i < FONT_HEIGHT * fontSize; i += fontSize)
    {
        l = font[FONT_HEIGHT * charId + count++];
        for (int j = 0; j < FONT_WIDTH * fontSize; j += fontSize)
        {
            if ((l & 0x01) == 0x01)
            {
                printSquare(x + j, y + i, fontSize, color);
            }
            l = l >> 1;
        }
    }
}

void printString(uint16_t x, uint16_t y, uint8_t *string, Color color)
{
    int i = 0;
    int j = 0;
    uint8_t c;
    uint8_t newLine = 10;
    while ((c = string[j++]) != 0)
    {
        if (((x + fontSize * (i * FONT_WIDTH + FONT_WIDTH)) > screenData->width) || c == newLine)
        {
            i = 0;
            if ((screenData->y_char + 0x01) * FONT_HEIGHT * fontSize >= screenData->height)
            {
                void *dst = (void *)((uint64_t)screenData->framebuffer);
                void *src = (void *)(dst + sizeof(Color) * fontSize * FONT_HEIGHT * (uint64_t)screenData->width);
                uint64_t length = sizeof(Color) * fontSize * ((uint64_t)screenData->width * (screenData->height - FONT_HEIGHT));
                memcpy(dst,src,length);
                memset(dst + length, 0, sizeof(Color) * fontSize * (uint64_t) screenData->width * FONT_HEIGHT);
            }
            else
            {
                screenData->y_char += 1;
            }
        }
        printChar(x + FONT_WIDTH * i * fontSize, screenData->y_char * FONT_HEIGHT * fontSize, c, color);
        if (c != newLine)
        {
            i++;
        }
    }
}

void fillrect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color)
{
    if (x >= screenData->width || y >= screenData->height)
    {
        return;
    }

    uint16_t maxX = screenData->width - x;
    if (w > maxX)
    {
        w = maxX;
    }

    uint16_t maxY = screenData->height - y;
    if (h > maxY)
    {
        h = maxY;
    }

    Color *where = getPixel(x, y);
    unsigned int lineBreak = screenData->width - w;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            *(where++) = color;
        }
        where += lineBreak;
    }
}

void *getPixel(uint16_t x, uint16_t y)
{
    return (void *)(screenData->framebuffer + sizeof(Color) * (x + (y * (uint64_t)screenData->width)));
}

void colorScreen(Color color)
{
    fillrect(0, 0, screenData->width, screenData->height, color);
}

void clearScreen()
{
    screenData->y_char = 0x00;
    Color black = {0x00, 0x00, 0x00};
    colorScreen(black);
}

void printHex(uint64_t value)
{
    printBase(value, (uint32_t)16);
}

void printDec(uint64_t value)
{
    printBase(value, (uint32_t)10);
}
void printBin(uint64_t value)
{
    printBase(value, (uint32_t)2);
}

void printBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    Color white = {0xff, 0xff, 0xff};
    printString(0, 0, buffer, white);
}

static uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base)
{
    uint8_t *p = buffer;
    uint8_t *p1, *p2;
    uint32_t digits = 0;

    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    *p = 0;

    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

uint16_t getHeight()
{
    return screenData->height;
}

uint16_t getWidth()
{
    return screenData->width;
}

void setFontSize(uint32_t size) {
    fontSize = size;
    clearScreen();
}