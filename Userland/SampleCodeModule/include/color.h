#ifndef _COLOR_H_
#define _COLOR_H_


typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}Color;

//Delete color's def. in KERNEL SPACE -> color.c
static Color black = {0x00, 0x00, 0x00};
static Color white = {0xFF, 0xFF, 0xFF};
static Color green = {0x1F, 0xED, 0x11};
static Color gray = {0x90, 0x90, 0x90};
static Color blue = {0xFF, 0x00, 0x00};
static Color red = {0x00, 0x00, 0xFF};


#endif
