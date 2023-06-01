#include <process.h>
#include <stdlib.h>
#include <string.h>
#include <scheduler.h>

uint64_t pidc = 0;

ProcessP newProcess(char * name, void * entryPoint, char ** argv, uint64_t priority){
    ProcessP proc = malloc(sizeof(Process));

    void * stack = malloc(STACK_FRAME_SIZE);

    char * stackBase = stack + STACK_FRAME_SIZE - sizeof(uint64_t);

    StackFrame stackFrame = createStack(entryPoint, stackBase, argv);

    memcpy(stack + STACK_FRAME_SIZE - sizeof(StackFrame), &stackFrame, sizeof(StackFrame));

    proc->name = name;
    proc->pid = pidc++;
    proc->ppid = get_running_pid();
    proc->sp = (uint64_t) stack + STACK_FRAME_SIZE - sizeof(StackFrame);
    proc->bp = (uint64_t) stackBase;
    proc->state = NEW;
    proc->stack = stack;
    proc->priority = priority;
    return proc;
}


