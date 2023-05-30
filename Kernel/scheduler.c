#include <scheduler.h>

ProcessP processes[2];
uint64_t counter = 0;
uint64_t current = 1;

extern void force_timer_tick();

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp){
    timer_handler();


    uint64_t te = ticks_elapsed();

    //if (processes[current]->state == NEW) {
    if (te == 1){
        processes[current]->state = RUNNING;
        // ((EntryPoint) processes[0]->entryPoint)();
    } else {

        processes[current]->sp = sp;
        if (te % 10 == 0) {
            current = 0;
        } else {
            current = 1;
        }
    }

    counter++;
    // printString("TICK", WHITE);

    // StackFrame * s = ((StackFrame *) processes[0]->stack);






    return processes[current]->sp;

}


void init_scheduler(){
    processes[0] = newProcess("IDLE", idleCodeModule);
    processes[1] = newProcess("SHELL", uCodeModuleAddress);
}

void killCurrentProcess(){
    force_timer_tick();
    return;
}



