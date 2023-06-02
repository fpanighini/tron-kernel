#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#include <timer.h>
#include <process.h>
#include <stack.h>
#include <memoryManager.h>
#include <stdint.h>
// #include <codeModules.h>
#include <videoDriver.h>

#define MAX_QUANTUM 5


typedef struct Node {
    uint64_t quantums;
    ProcessP proc;
    struct Node * next;
} Node;

typedef Node * NodeP;

uint64_t scheduler(uint64_t sp);

void killCurrentProcess(void);

void init_scheduler();

void add_process(char * name, void * program, char ** argv, uint64_t priority);

void block_current_process();

void block_process(uint64_t pid);

void ready_process(uint64_t pid);

uint64_t get_running_pid(void);

#endif
