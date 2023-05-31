#include "include/timer.h"
#include "include/videoDriver.h"
#include <scheduler.h>

ProcessP processes[2];
uint64_t counter = 0;
uint64_t current = 0;

extern void force_timer_tick();

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp){
    if (counter == 1){
        return sp;
    }
    uint64_t te = ticks_elapsed();
    processes[current]->sp = sp;
    current = ((te % 10) == 0) ? 0 : 1;
    return processes[current]->sp;

}

void idle(){
    while(1){
        wait(1);
        printString("IDLE", WHITE);
    }
}



void idle2(){
    while(1){
        wait(1);
        printString("ooooo", WHITE);
    }
}

void init_scheduler(){
    add_process("IDLE", &idle);
}

void add_process(char * name, void * program){
    processes[counter++] = newProcess(name, program);
}



void killCurrentProcess(){
    force_timer_tick();
    return;
}



