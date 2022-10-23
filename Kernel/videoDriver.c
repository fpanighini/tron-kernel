#include <videoDriver.h>
#include <font.h>

static uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);

static uint8_t buffer[64] = {'0'};

static struct vbe_mode_info_structure *screenData = (void *)0x5C00;

void printSquare(uint16_t x, uint16_t y, uint16_t l, Color color)
{
    fillrect(x, y, l, l, color);
}

void printChar(uint16_t x, uint16_t y, uint8_t c, uint16_t size, Color color)
{
    if (c < FIRST_CHAR || c > LAST_CHAR)
    {
        return;
    }
    int charId = c - FIRST_CHAR;
    char l;
    int count = 0;

    for (int i = 0; i < FONT_HEIGHT * size; i += size)
    {
        l = font[FONT_HEIGHT * charId + count++];
        for (int j = 0; j < FONT_WIDTH * size; j += size)
        {
            if ((l & 0x01) == 0x01)
            {
                printSquare(x + j, y + i, size, color);
            }
            l = l >> 1;
        }
    }
}

void printString(uint16_t x, uint16_t y, uint8_t *string, uint16_t size, Color color)
{
    int i = 0;
    int j = 0;
    uint8_t c;
    uint8_t newLine = 10;
    uint8_t bcp = screenData->y_char;
    while ((c = string[j++]) != 0)
    {
        if (((x + size * (i * FONT_WIDTH + FONT_WIDTH)) > screenData->width) || c == newLine)
        {
            i = 0;
            screenData->y_char += size * FONT_HEIGHT;
            if (screenData->y_char >= screenData->height)
            {
                screenData->y_char = bcp;
            }
        }
        printChar(x + FONT_WIDTH * i * size, screenData->y_char, c, size, color);
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
    return (void *)(screenData->framebuffer + 3 * (x + (y * (uint64_t)screenData->width)));
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

void putpixel(unsigned char *screen, int x, int y, int color)
{
    unsigned where = 3 * (x + y * screenData->width);
    screen[where] = color & 255;             // BLUE
    screen[where + 1] = (color >> 8) & 255;  // GREEN
    screen[where + 2] = (color >> 16) & 255; // RED
}

void printHex(uint64_t value)
{
    printBase(value, 16);
}

void printDec(uint64_t value)
{
    printBase(value, 10);
}
void printBin(uint64_t value)
{
    printBase(value, 2);
}

void printBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    Color white = {0xff, 0xff, 0xff};
    printString(0, 0, buffer, 1, white);
}

static uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base)
{
    uint8_t *p = buffer;
    uint8_t *p1, *p2;
    uint32_t digits = 0;

    // Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    // Reverse string in buffer.
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

char getKey(int id)
{
    if (id >= 128)
    {
        return -1;
    }
    char kbd_US[128] =
        {
            0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
            '\t', /* <-- Tab */
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
            0, /* <-- control key */
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
            '*',
            0,   /* Alt */
            ' ', /* Space bar */
            0,   /* Caps lock */
            0,   /* 59 - F1 key ... > */
            0, 0, 0, 0, 0, 0, 0, 0,
            0, /* < ... F10 */
            0, /* 69 - Num lock*/
            0, /* Scroll Lock */
            0, /* Home key */
            0, /* Up Arrow */
            0, /* Page Up */
            '-',
            0, /* Left Arrow */
            0,
            0, /* Right Arrow */
            '+',
            0, /* 79 - End key*/
            0, /* Down Arrow */
            0, /* Page Down */
            0, /* Insert Key */
            0, /* Delete Key */
            0, 0, 0,
            0, /* F11 Key */
            0, /* F12 Key */
            0, /* All other keys are undefined */
        };

    return kbd_US[id];
}
