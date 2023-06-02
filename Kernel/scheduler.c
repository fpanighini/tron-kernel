#include "include/timer.h"
#include "include/videoDriver.h"
#include <scheduler.h>
#include <stdint.h>


void add_node(ProcessP process);
NodeP find_next_ready(NodeP current);
NodeP find_node(uint64_t pid);

void destroy_current_node();
void free_node(NodeP node);

uint64_t disable_count = 0;

NodeP first = NULL;
NodeP currentNode = NULL;

uint64_t counter = 0;
uint64_t force_yield = 0;

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp){
    if(disable_count) {
        return currentNode->proc->sp = sp;
    }

    if (counter == 1){
        return sp;
    }

    // Save the current process stack pointer
    currentNode->proc->sp = sp;

    // If process is running increase its quantums
    if (currentNode->proc->state == RUNNING){
        currentNode->quantums++;
    } else if (currentNode->proc->state == KILLED){ // If process is DEAD destroy the currentNode (replaced by the next ready process)
        destroy_current_node();
        currentNode = find_next_ready(currentNode);
        currentNode->proc->state = RUNNING;
        currentNode->quantums = currentNode->proc->priority;
        return currentNode->proc->sp;
    }

    if (currentNode->quantums > MAX_QUANTUM || currentNode->proc->state == BLOCKED || force_yield){
        force_yield = 0;
        currentNode->proc->state = READY;
        currentNode = find_next_ready(currentNode->next);
        currentNode->proc->state = RUNNING;
        currentNode->quantums = currentNode->proc->priority;
    }

    return currentNode->proc->sp;
}

NodeP find_next_ready(NodeP current){
    if (current->proc->state == READY || current->proc->state == NEW){
        return current;
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
    newNode->proc = newProcess("IDLE", &idle, argv, MAX_QUANTUM);
    newNode->quantums = MAX_QUANTUM;
    first = newNode;
    newNode->next = first;
    currentNode = first;
    counter++;
    currentNode->proc->state = RUNNING;
    // add_process("IDLE", &idle);
}

uint64_t add_process(char * name, void * program, char ** argv, uint64_t priority){
    ProcessP proc = newProcess(name, program, argv, priority);
    add_node(proc);
    counter++;
    return proc->pid;
}


void add_node(ProcessP process){
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printString((uint8_t *) "MemError", RED);
        return ;
    }
    newNode->quantums = process->priority;
    newNode->proc = process;
    newNode->next = currentNode->next;
    currentNode->next = newNode;
}



void killCurrentProcess(){
    currentNode->proc->state = KILLED;
    _force_scheduler();
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

uint64_t kill_process(uint64_t pid){
    NodeP node = find_node(pid);
    if (node == NULL){
        return -1;
    }
    node->proc->state = KILLED;
    return node->proc->pid;
}

void block_current_process(){
    currentNode->proc->state = BLOCKED;
    // scheduler();
}

uint64_t block_process(uint64_t pid){
    NodeP node = find_node(pid);
    if (node == NULL){
        return -1;
    }
    node->proc->state = BLOCKED;
    return node->proc->pid;
}

uint64_t ready_process(uint64_t pid){
    NodeP node = find_node(pid);
    if (node == NULL){
        return -1;
    }
    node->proc->state = READY;
    return node->proc->pid;
}

uint64_t get_running_pid(void){
    return currentNode == NULL ? 0 : currentNode->proc->pid;
}

void destroy_current_node(){
    currentNode->proc = currentNode->next->proc;
    currentNode->next = currentNode->next->next;
    free_node(currentNode->next);
}

void free_node(NodeP node){
    free_proc(node->proc);
    free(node);
}

void scheduler_enable() {
    if(disable_count > 0)
        disable_count--;
}

void scheduler_disable() {
    int count;
    do count = disable_count; 
    while(_cmpxchg(&disable_count, count+1, count) != count);
}

void force_current_yield(){
    force_yield = 1;
    _force_scheduler();
}

//TODO: ojo con la declaracion de xchg (libasm.h)
