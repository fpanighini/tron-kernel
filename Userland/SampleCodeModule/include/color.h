#ifndef _COLOR_H_
#define _COLOR_H_

typedef struct {
	char b;
	char g;
	char r;
}Color;

//Delete color's def. in KERNEL SPACE -> color.c
extern Color black;
extern Color white;
extern Color green;
extern Color gray;
extern Color blue;
extern Color red;
extern Color magenta;


#endif
