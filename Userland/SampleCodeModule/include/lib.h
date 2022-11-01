#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>
#include <color.h>

typedef uint8_t bool;

#define true 1
#define false 0


// Borrar las que no se usan:
int strcmp(const char *str1, const char *str2);
int strncmp (const char *str1, const char *str2, char n);
int strlen(const char *str1);
char * strcat(char *dest, const char *src);
char * strncat(char *dest, const char *src, int n);
char * strcpy(char * dest, const char *src);
char * strncpy(char * dest, const char *src, int n);
long itoa(long number, char *str);
int getChar(void);
void printf(const char *fmt, ...);
int putColorChar(int car, Color c);
int putChar(int c);
void putStringColor(char *str, Color c);
void puts(char *fmt);
void scanf(int length, const char *fmt, ...);
void printStringAt(int x, int y, char *str, Color c);
void clear();
int getTimedChar();
long getScreenWidth();
long getScreenHeight();
long drawRectangle(int x, int y, int width, int height, Color color);
void shortSleep();
void printBase(uint64_t value, uint32_t base);
void beep(int frequency);

#endif
