// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/scheduler.h"
#include <syscallManager.h>
#include "include/process.h"
#include "include/timer.h"
#include "include/videoDriver.h"
#include <scheduler.h>
#include <semaphore.h>
#include <stdint.h>

#define TAB "    "
#define SCHED_MUTEX "SCHEDULER_MUTEX"
#define PROC_MUTEX "PROC_MUTEX"

NodeP add_node(ProcessP process);
NodeP find_next_ready(NodeP current);
NodeP find_node(uint64_t pid);

void destroy_current_node();
void destroy_node(NodeP node);
void free_node(NodeP node);
void destroy_ground_node(uint64_t pid);
void kill_foreground_proc();

uint64_t disable_count = 0;

NodeP first = NULL;
NodeP currentNode = NULL;

NodeP foreground = NULL;
NodeP background = NULL;

uint64_t counter = 0;
uint64_t block_count = 0;

uint64_t force_yield = 0;

typedef int (*EntryPoint)();

uint64_t scheduler(uint64_t sp)
{
    if (disable_count)
    {
        return currentNode->proc->sp = sp;
    }

    if (counter == 1)
    {
        return sp;
    }

    // Save the current process stack pointer
    currentNode->proc->sp = sp;

    // If process is running increase its quantums
    if (currentNode->proc->state == RUNNING)
    {
        currentNode->quantums++;
    }
    else if (currentNode->proc->state == KILLED) // If process is DEAD destroy the currentNode (replaced by the next ready process)
    {
        if (currentNode->proc == foreground->proc)
        {
            kill_foreground_proc();
        }
        else
        {
            destroy_node(currentNode);
        }
        currentNode = find_next_ready(currentNode);
        currentNode->proc->state = RUNNING;
        currentNode->quantums = currentNode->proc->priority;
        return currentNode->proc->sp;
    }
    if (currentNode->proc->state == BLOCKED)
    {
        currentNode = find_next_ready(currentNode->next);
        currentNode->proc->state = RUNNING;
        currentNode->quantums = currentNode->proc->priority;
    }
    else if (currentNode->quantums > MAX_QUANTUM || force_yield)
    {
        force_yield = 0;
        currentNode->proc->state = READY;
        currentNode = find_next_ready(currentNode->next);
        currentNode->proc->state = RUNNING;
        currentNode->quantums = currentNode->proc->priority;
    }

    return currentNode->proc->sp;
}

NodeP find_next_ready(NodeP current)
{
    if (current->proc->state == READY || current->proc->state == NEW)
    {
        return current;
    }
    return find_next_ready(current->next);
}

void idle()
{
    while (1)
    {
        sys_wait(100);
    }
}

void init_scheduler()
{
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printString((uint8_t *)"MemError", RED);
        return;
    }
    char *argv[] = {NULL};
    newNode->proc = newProcess("IDLE", &idle, argv, 0, 1, MAX_QUANTUM);
    newNode->quantums = MAX_QUANTUM;
    first = newNode;
    newNode->next = first;
    currentNode = first;
    currentNode->proc->state = RUNNING;
    counter++;
    sem_open(PIDC_MUTEX, 1);
    sem_open(SCHED_MUTEX, 1);
    sem_open(PROC_MUTEX, 1);
}

uint64_t add_process(char *name, void *program, char **argv, uint64_t read_fd, uint64_t write_fd, uint64_t priority, uint64_t is_foreground)
{
    scheduler_disable();

    if (priority > 5)
    {
        priority = 5;
    }

    ProcessP proc = newProcess(name, program, argv, read_fd, write_fd, priority);

    sem_wait(PROC_MUTEX);
    currentNode->proc->children++;
    sem_post(PROC_MUTEX);

    NodeP newNode = add_node(proc);

    if (is_foreground == 1)
    {
        if (background == NULL)
        {
            background = foreground;
        }
        foreground = newNode;
    }

    counter++;
    scheduler_enable();
    return proc->pid;
}

void ready_foreground_proc()
{
    foreground->proc->state = foreground->proc->state == BLOCKED ? READY : foreground->proc->state;
}

void kill_foreground_proc()
{
    if (background != NULL)
    {
        NodeP aux = foreground;
        foreground = background;

        ready_foreground_proc();

        foreground->proc->read_fd = 0;
        background = NULL;
        kill_process(aux->proc->pid);
    }
}

NodeP add_node(ProcessP process)
{
    NodeP newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printString((uint8_t *)"MemError", RED);
        return 0;
    }
    newNode->quantums = process->priority;
    newNode->proc = process;
    newNode->next = currentNode->next;
    currentNode->next = newNode;
    return newNode;
}

void killCurrentProcess()
{
    currentNode->proc->state = KILLED;
    _force_scheduler();
    return;
}

NodeP find_node(uint64_t pid)
{
    if (first->proc->pid == pid)
    {
        return first;
    }
    NodeP current = first->next;
    while (current != first)
    {
        if (current->proc->pid == pid)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

uint64_t kill_process(uint64_t pid)
{
    if (currentNode->proc->pid == pid)
    {
        killCurrentProcess();
    }

    NodeP node = find_node(pid);
    if (node == NULL)
    {
        printString((uint8_t *)"PID NOT FOUND\n", RED);
        return -1;
    }
    node->proc->state = KILLED;
    uint64_t removedPid = node->proc->pid;
    destroy_node(node);
    return removedPid;
}

uint64_t change_priority(uint64_t pid, uint64_t priority)
{
    NodeP node = find_node(pid);
    if (node == NULL)
    {
        return -1;
    }
    node->proc->priority = priority;
    return node->proc->pid;
}

void block_current_process()
{
    block_count++;
    currentNode->proc->state = BLOCKED;
    _force_scheduler();
}

uint64_t block_process(uint64_t pid)
{
    NodeP node = find_node(pid);
    if (node == NULL)
    {
        return -1;
    }
    node->proc->state = BLOCKED;
    _force_scheduler();
    return node->proc->pid;
}

uint64_t ready_process(uint64_t pid)
{
    NodeP node = find_node(pid);
    if (node == NULL)
    {
        return -1;
    }
    node->proc->state = READY;
    block_count--;
    return node->proc->pid;
}

uint64_t get_running_pid(void)
{
    return currentNode == NULL ? 0 : currentNode->proc->pid;
}

void destroy_node(NodeP node)
{
    NodeP parent = find_node(currentNode->proc->ppid);
    if (parent == NULL)
        return;

    sem_wait(PROC_MUTEX);
    parent->proc->children--;
    if (parent->proc->children == 0)
    {
        parent->proc->blocked_by_children = 0;
        ready_process(parent->proc->pid);
    }
    sem_post(PROC_MUTEX);

    counter--;
    if (node->proc->state == BLOCKED)
    {
        block_count--;
    }

    NodeP aux = node->next;
    free_proc(node->proc);
    node->proc = node->next->proc;

    node->next = node->next->next;

    if (node->proc->pid == 0)
    {
        first = node;
    }

    free(aux);
}

void destroy_current_node()
{

    NodeP aux = currentNode->next;
    free_proc(currentNode->proc);
    currentNode->proc = currentNode->next->proc;

    currentNode->next = currentNode->next->next;

    if (currentNode->proc->pid == 0)
    {
        first = currentNode;
    }

    free(aux);
}

void free_node(NodeP node)
{
    free_proc(node->proc);
    free(node);
}

void scheduler_enable()
{
    if (disable_count > 0)
        disable_count--;
}

void scheduler_disable()
{
    int count;
    do
        count = disable_count;
    while (_cmpxchg(&disable_count, count + 1, count) != count);
}

void force_current_yield()
{
    force_yield = 1;
    _force_scheduler();
}

ProcessP get_current_proc()
{
    return currentNode->proc;
}

uint64_t get_current_read()
{
    return currentNode->proc->read_fd;
}

uint64_t get_current_write()
{
    return currentNode->proc->write_fd;
}

void printNode(NodeP node)
{
    ProcessP proc = node->proc;

    printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%s\n", proc->name, proc->pid, proc->priority, proc->bp, proc->sp, proc->state, proc->pid == foreground->proc->pid ? "FOREGROUND" : "BACKGROUND");
}

void print_all_nodes(void)
{
    printf("NAME\t\tPID\t\tPRIORITY\t\tBP\t\tSP\t\tSTATE\t\tGROUND\n");
    NodeP cur = first;
    printNode(cur);
    cur = cur->next;

    while (cur != first)
    {
        printNode(cur);
        cur = cur->next;
    }
}
