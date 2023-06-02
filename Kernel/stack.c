#include "include/videoDriver.h"
#include <stack.h>
#include <scheduler.h>
#include <process.h>
#include <videoDriver.h>

StackFrame createStack(void * entryPoint, void * stackBase, uint64_t argc, char ** argv){
    // for (int i = 0 ; i < count_argv(argv); i++){
    //     printString(argv[i], WHITE);
    // }
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
    stackFrame.rdi = argc;
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

