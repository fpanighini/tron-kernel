#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#include <timer.h>
#include <process.h>
#include <stack.h>
#include <memoryManager.h>
#include <stdint.h>
#include <codeModules.h>
#include <videoDriver.h>

uint64_t scheduler(uint64_t sp);

void killCurrentProcess(void);

void init_scheduler();


#endif
