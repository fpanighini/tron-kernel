#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#include <timer.h>
#include <process.h>
#include <stack.h>
#include <memoryManager.h>
#include <stdint.h>
// #include <codeModules.h>
#include <videoDriver.h>
#include <semaphore.h>

#define MAX_QUANTUM 5


typedef struct Node {
    uint64_t quantums;
    ProcessP proc;
    struct Node * next;
} Node;

typedef Node * NodeP;

// ASM functions
void _force_scheduler();

uint64_t _xchg(uint64_t *dest, uint64_t value);

uint64_t _cmpxchg(uint64_t *dest, uint64_t value, uint64_t test);


uint64_t scheduler(uint64_t sp);

void killCurrentProcess(void);

void init_scheduler();

uint64_t add_process(char * name, void * program, char ** argv, uint64_t read_fd, uint64_t write_fd, uint64_t priority);

uint64_t kill_process(uint64_t pid);

uint64_t change_priority(uint64_t pid, uint64_t priority);

void block_current_process();

uint64_t block_process(uint64_t pid);

uint64_t ready_process(uint64_t pid);

uint64_t get_running_pid(void);

void scheduler_enable();

void scheduler_disable();

void force_current_yield();

uint64_t get_current_read();

uint64_t get_current_write();

void print_all_nodes();

#endif
