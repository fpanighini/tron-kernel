#ifndef _LIB_H_
#define _LIB_H_

int strcmp(const char *str1, const char *str2);
int strncmp (const char *str1, const char *str2, char n);
int strlen(const char *str1);
char * strcat(char *dest, const char *src);
char * strncat(char *dest, const char *src, int n);
char * strcpy(char * dest, const char *src);
char * strncpy(char * dest, const char *src, int n);
long atoi(long number, char *str);

#endif