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
int scanf(const char *fmt, ...);

#endif
