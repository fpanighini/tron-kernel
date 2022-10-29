#ifndef _LIB_H_
#define _LIB_H_

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
int printf(const char *fmt, ...);
int putChar(int c);
int scanf(const char *fmt, ...);

#endif
