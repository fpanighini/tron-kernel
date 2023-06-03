#ifndef _SYSCALLS_USERLAND_H_
#define _SYSCALLS_USERLAND_H_
#include <color.h>
#include <stdint.h>

#define STDOUT 0
#define STDERR 1
#define STDIN 0

#define PROC_MAX 50
#define NAME_LIMIT 1024

typedef struct semInfo {
    int semId;
    char name[NAME_LIMIT];
    uint64_t lock;   // lock
    int value;
    uint64_t openCount;    // Counter of pending 'close' signals
    uint64_t blockedProcesses[PROC_MAX];
    uint64_t blockedFirst;
    uint64_t blockedLast;
} semInfo_t;

long sys_read(unsigned char fd, char * s, int count);
long sys_write(unsigned char fd, char * s, Color c);
long sys_writeAt(short x, short y, char * s, Color c);
long sys_clearScreen();
long sys_wait(int ticks);
long sys_time();
long sys_date();
long sys_beep();
long sys_getScreenHeight();
long sys_getScreenWidth();
long sys_timedRead(unsigned char fd, char * s, int count, int millis);
long sys_drawRectangle(int x, int y, int width, int height, Color color);
long sys_inforeg(long * registers);
long sys_changeFontSize(int diff);
void * sys_malloc(int size);
void sys_free(void * ptr);
long sys_exec(char * name, void * program, char ** argv, int read_fd, int write_fd, int priority);
long sys_pipe_open(char* name);
long sys_pipes_info();
long sys_pipe_close(int id);
long sys_sem_open(char *name, int value);
long sys_sem_close(char *name);
long sys_sem_post(char *name);
long sys_sem_wait(char *name);
long sys_sem_info(int idx, semInfo_t *buffer);
long sys_sem_count();
long sys_get_pid();
long sys_kill(int pid);
long sys_block(int pid);
long sys_unblock(int pid);
void sys_yield(void);

#endif
