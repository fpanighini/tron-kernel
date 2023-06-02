#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

char keyPressed(void);

long itoa(long number, char *str);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp(char * str1, char * str2);
int strlen(char * str);

#endif
