#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include "memoryManager.h"
// #include "scheduler.h"
//  #include <scheduler.h>
#include <stack.h>

#define PIDC_MUTEX "PROCESSES_MUTEX"

#define STACK_FRAME_SIZE 4000

typedef enum
{
    READY = 0,
    RUNNING,
    BLOCKED,
    NEW,
    KILLED
} states;

typedef struct Process
{
    char *name;
    char **argv;
    uint64_t pid;
    uint64_t ppid;
    uint64_t sp;
    uint64_t bp;
    states state;
    void *stack;
    uint64_t read_fd;
    uint64_t write_fd;
    uint64_t priority;
} Process;

typedef struct Process *ProcessP;

ProcessP newProcess(char *name, void *entryPoint, char **argv, uint64_t read_fd, uint64_t write_fd, uint64_t priority);

void free_proc(ProcessP proc);

#endif
