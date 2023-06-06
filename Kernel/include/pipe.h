#ifndef PIPE_H_
#define PIPE_H_
#include <scheduler.h>
#include <stdint.h>
#include <process.h>
#include <lib.h>
#include <semaphore.h>

#define MAX_PIPES 10
#define PIPE_SIZE 4096
#define PROCS 10
#define NAME_MAX 100
typedef struct
{
    char name[NAME_MAX];
    int created;
    char data[PIPE_SIZE];
    int rProcesses[PROCS];
    int wProcesses[PROCS];
    unsigned int nread;
    unsigned int nwrite;
    int usingPipe;
    int waitingPid;
    int fd;
} pipe_t;
typedef int (*function_t)(size_t argc, char **argv);

int pipe_read(int index, char *address, int n);
int pipe_write(int index, char *address, int n);
void pipe_close(int index);
int pipe_open(char *name);
char *pipes_info();
#endif