#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>
#include <color.h>
#include <syscalls.h>

#define BUFF_SIZE 100

#define NULL ((void *)0)

#define COMMAND_CHAR "$> "
#define CURSOR "|"
#define BACKSPACE '\b'
#define MAX_KBD_BUF 55
#define MAX_TERMINAL_CHARS 124 // 124 = (1024/8) - 4 (number of characters that fit in one line minus the command prompt and cursor characters)

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define REGISTER_NUM 17

#define true 1
#define false 0

typedef uint8_t bool;

int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, int n);
int strlen(const char *str1);
char *strcat(char *dest, const char *src);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int n);
char *strchr(const char *str, int c);
long itoa(long number, char *str);
int atoi(const char *str);

int inputRead(char **buf);
void bufferRead(char **buf);
int getChar(void);
void printf(const char *fmt, ...);
int putColorChar(int car, Color c);
int putChar(int c);
void putStringColor(char *str, Color c);
void puts(char *fmt);
void scanf(int length, const char *fmt, ...);
void printStringAt(int x, int y, char *str, Color c);
void printString(char *str, Color c);
void printError(char *str);
void clear();
int getTimedChar();
long getScreenWidth();
long getScreenHeight();
long drawRectangle(int x, int y, int width, int height, Color color);
void shortSleep(int ticks);
void printBase(uint64_t value, uint32_t base);
uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);
void beep(int frequency);
void *malloc(unsigned int memSize);
void free(void *ptr);
long exec(char *name, void *program, char **argv, int read_fd, int write_fd, int priority, int is_foreground);
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
char *pipes_info();
int pipe_open(char *name);
void pipe_close(int index);
void yield();
long read(uint8_t fd, char *buf, int count);
long write(uint8_t fd, char *string, long n, Color color);
long infoReg(long array[REGISTER_NUM]);
void ps();
long changeFontSize(uint32_t size);
long time();
long date();
void wait_pid();

#endif
