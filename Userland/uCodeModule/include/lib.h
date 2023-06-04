#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>
#include <color.h>
#include <syscalls.h>


typedef uint8_t bool;

#define true 1
#define false 0

#define NULL ((void *)0)

int strcmp(const char *str1, const char *str2);
int strncmp (const char *str1, const char *str2, int n);
int strlen(const char *str1);
char * strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int n);
char* strchr(const char* str, int c);
long itoa(long number, char *str);
int atoi(const char *str);

int getChar(void);
void printf(const char *fmt, ...);
int putColorChar(int car, Color c);
int putChar(int c);
void putStringColor(char *str, Color c);
void puts(char *fmt);
void scanf(int length, const char *fmt, ...);
void printStringAt(int x, int y, char *str, Color c);
void printString(char *str, Color c);
void printError(char * str);
void clear();
int getTimedChar();
long getScreenWidth();
long getScreenHeight();
long drawRectangle(int x, int y, int width, int height, Color color);
void shortSleep(int ticks);
void printBase(uint64_t value, uint32_t base);
void beep(int frequency);
void * malloc(unsigned int memSize);
void free(void * ptr);
long exec(char * name, void * program, char ** argv, int read_fd, int write_fd, int priority);
long get_pid();
long kill(int pid);
long change_priority(int pid, int priority);
long block(int pid);
long unblock(int pid);
int sem_open(char *semName, int initValue);
int sem_wait(char *semName);
int sem_post(char *semName);
int sem_close(char *semName);
int sem_info(int idx, semInfo_t *buffer);
int sems_count();
char * pipes_info();
int pipe_open(char* name);
void pipe_close(int index);
void yield();

#endif
