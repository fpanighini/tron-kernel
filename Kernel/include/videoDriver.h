#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_

#include <stdint.h>
typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
}Color;

extern Color white;
extern Color green;
extern Color gray;
extern Color black;
extern Color blue;
extern Color red;

void setFontSize(uint32_t size);

void* getPixel(uint16_t x, uint16_t y);

void fillrect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color);

void colorScreen(Color color);

void clearScreen();

void printSquare(uint16_t x, uint16_t y, uint16_t l, Color color);

void printChar(uint16_t x, uint16_t y, uint8_t c, Color color);

void printString(uint16_t x, uint16_t y, uint8_t * string, Color color);

void printHex(uint64_t value);

void printDec(uint64_t value);

void printBin(uint64_t value);

void printBase(uint64_t value, uint32_t base);

uint16_t getHeight();

uint16_t getWidth();

#endif