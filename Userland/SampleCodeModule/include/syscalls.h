#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_
#include <color.h>

long sys_read(unsigned char fd, char * s, int count);
long sys_write(char * s, Color c);
long sys_writeAtX(int x, char * s, Color c);
long sys_clearScreen();
long sys_wait(int ticks);

#endif
