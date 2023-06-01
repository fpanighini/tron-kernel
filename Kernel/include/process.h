#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include "memoryManager.h"
// #include <scheduler.h>
#include <stack.h>


#define STACK_FRAME_SIZE 4000

typedef enum {READY = 0, RUNNING, BLOCKED, NEW} states;

typedef struct Process {
    char * name;
    uint64_t pid;
    uint64_t ppid;
    uint64_t sp;
    uint64_t bp;
    states state;
    void * stack;
    uint64_t priority;
} Process;

typedef Process * ProcessP;

ProcessP newProcess(char * name, void * entryPoint, char ** argv, uint64_t priority);

#endif
