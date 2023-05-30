#include "include/videoDriver.h"
#include <scheduler.h>

ProcessP processes[2];
uint64_t counter = 0;
uint64_t current = 0;

extern void force_timer_tick();

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp){
    timer_handler();
    if (counter == 1){
        // processes[current]->sp = sp;
        return sp;
    }


    uint64_t te = ticks_elapsed();

    //if (processes[current]->state == NEW) {
    if (te == 1){
        processes[current]->state = RUNNING;
        // ((EntryPoint) processes[0]->entryPoint)();
    } else {

        processes[current]->sp = sp;
        if (te % 3 == 0) {
            current = 0;
        } else {
            current = 1;
        }
    }

    // printString("TICK", WHITE);

    // StackFrame * s = ((StackFrame *) processes[0]->stack);
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



