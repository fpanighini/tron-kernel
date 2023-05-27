#include <process.h>
#include <string.h>

uint64_t pidc = 0;


ProcessP newProcess(char * name, void * entryPoint){
    ProcessP proc = malloc(sizeof(Process));

    void * stack = malloc(STACK_FRAME_SIZE);


    char * stackBase = stack + STACK_FRAME_SIZE - sizeof(uint64_t);

    StackFrame stackFrame = createStack(entryPoint, stackBase);


    memcpy(stack + STACK_FRAME_SIZE - sizeof(StackFrame), &stackFrame, sizeof(StackFrame));

    StackFrame  * t = (StackFrame *) stackBase;

    proc->name = name;
    proc->pid = pidc++;
    proc->entryPoint = entryPoint;
    proc->sp = stack + STACK_FRAME_SIZE - sizeof(StackFrame);
    proc->bp = stackBase;
    proc->state = NEW;
    proc->stack = stack;
    return proc;
}


