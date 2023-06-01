#include "include/timer.h"
#include "include/videoDriver.h"
#include <scheduler.h>
#include <stdint.h>


void add_node(ProcessP process);
NodeP find_next_ready(NodeP current);
NodeP find_node(uint64_t pid);

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
        // _hlt();
        printString((uint8_t *) "IDLE", WHITE);
    }
}

void init_scheduler(){
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL){
    printString((uint8_t *) "MemError", RED);
        return ;
    }
    char * argv[] = {NULL};
    newNode->proc = newProcess("IDLE", &idle, argv, 0);
    first = newNode;
    newNode->next = first;
    currentNode = first;
    counter++;
    currentNode->proc->state = BLOCKED;
    // add_process("IDLE", &idle);
}

void add_process(char * name, void * program, char ** argv, uint64_t priority){
    add_node(newProcess(name, program, argv, priority));
    counter++;
}


void add_node(ProcessP process){
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printString((uint8_t *) "MemError", RED);
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

NodeP find_node(uint64_t pid){
    if (first->proc->pid == pid){
        return first;
    }
    NodeP current = first->next;
    while (current != first){
        if (current->proc->pid == pid){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void block_current_process(){
    currentNode->proc->state = BLOCKED;
    // scheduler();
}

void block_process(uint64_t pid){
    NodeP node = find_node(pid);
    node->proc->state = BLOCKED;
}

void ready_process(uint64_t pid){
    NodeP node = find_node(pid);
    node->proc->state = READY;
}

uint64_t get_running_pid(void){
    return currentNode == NULL ? 0 : currentNode->proc->pid;
}

