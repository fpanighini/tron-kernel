#include "include/timer.h"
#include "include/videoDriver.h"
#include <scheduler.h>
#include <stdint.h>


void add_node(ProcessP process);
NodeP find_next_ready(NodeP current);

NodeP first = NULL;
NodeP currentNode = NULL;

uint64_t counter = 0;

extern void force_timer_tick();

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp){
    if (counter == 1){
        return sp;
    }
    uint64_t te = ticks_elapsed();
    currentNode->proc->sp = sp;
    if((te % 1) == 0){
        currentNode->proc->state = currentNode->proc->state == BLOCKED ? BLOCKED : READY;
        currentNode = find_next_ready(currentNode);
        currentNode->proc->state = RUNNING;
    }
    return currentNode->proc->sp;
}

NodeP find_next_ready(NodeP current){
    if (current->next->proc->state == READY || current->next->proc->state == NEW){
        return current->next;
    }
    return find_next_ready(current->next);
}


void idle(){
    while(1){
        _hlt();
        printString((uint8_t) "IDLE", WHITE);
    }
}

void init_scheduler(){
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL){
    printString("MemError", RED);
        return 0;
    }
    newNode->proc = newProcess("IDLE", &idle);
    first = newNode;
    newNode->next = first;
    currentNode = first;
    counter++;
    currentNode->proc->state = BLOCKED;
    // add_process("IDLE", &idle);
}

void add_process(char * name, void * program){
    add_node(newProcess(name, program));
    counter++;
}


void add_node(ProcessP process){
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printString("MemError", RED);
        return ;
    }
    newNode->proc = process;
    newNode->next = currentNode->next;
    currentNode->next = newNode;
}



void killCurrentProcess(){
    force_timer_tick();
    return;
}



