#include <stack.h>
#include <scheduler.h>
#include <process.h>
#include <videoDriver.h>

uint64_t count_argv(char ** argv);

StackFrame createStack(void * entryPoint, void * stackBase, char ** argv){
    StackFrame stackFrame;

    stackFrame.r15 = 0x0;
    stackFrame.r14 = 0x0;
    stackFrame.r13 = 0x0;
    stackFrame.r12 = 0x0;
    stackFrame.r11 = 0x0;
    stackFrame.r10 = 0x0;
    stackFrame.r9 = 0x0;
    stackFrame.r8 = 0x0;
    stackFrame.rsi = (uint64_t) argv;
    stackFrame.rdi = count_argv(argv);
    stackFrame.rbp = 0x0;
    stackFrame.rdx = 0x0;
    stackFrame.rcx = 0x0;
    stackFrame.rbx = 0x0;
    stackFrame.rax = 0x0;
    stackFrame.ip = entryPoint;
    stackFrame.cs = 0x8;
    stackFrame.eflags = 0x202;
    stackFrame.sp = (uint64_t) stackBase;
    stackFrame.ss = 0x0;
    stackFrame.returnAddress = (uint64_t)&killCurrentProcess;

    return stackFrame;
}

uint64_t count_argv(char ** argv){
    uint64_t count = 0;
    while(argv[count] != NULL){
        count++;
    }
    return count;
}


