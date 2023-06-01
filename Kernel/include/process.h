#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include "memoryManager.h"
// #include <scheduler.h>
#include <stack.h>


#define STACK_FRAME_SIZE 2048

typedef enum {READY = 0, RUNNING, BLOCKED, NEW} states;

typedef struct Process {
    char * name;
    void * entryPoint;
    uint64_t pid;
    uint64_t sp;
    uint64_t bp;
    states state;
    void * stack;
} Process;

typedef Process * ProcessP;

ProcessP newProcess(char * name, void * entryPoint);

#endif
