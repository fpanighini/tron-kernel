#include <syscallManager.h>
#include <videoDriver.h>



static uint64_t sys_write() {
    printString(0,0, "sys_write\n", 2, penColor);
    return 0;
}

static uint64_t sys_read() {
    printString(0,0, "sys_read\n", 2, penColor);
    return 0;
}

static uint64_t sys_time() {
    printString(0,0, "sys_time\n", 2, penColor);
    return 0;
}

static uint64_t sys_date() {
    printString(0,0, "sys_date\n", 2, penColor);
    return 0;
}

static uint64_t sys_clearScreen() {
    clearScreen();
    return 0;
}

static uint64_t sys_drawLine() {
    printString(0,0, "sys_drawLine\n", 2, penColor);
    return 0;

}

static uint64_t sys_bell() {
    printString(0,0, "sys_bell\n", 2, penColor);
    return 0;
}



static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = { sys_write, sys_read, sys_time ,sys_date , sys_clearScreen , sys_drawLine , sys_bell  };

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    printString(0,0, "sys_handler\n", 2, penColor);
    if ((rax < SYSCALL_NUMBER) && syscall_handlers[rax] != 0)
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
    return 0;
}

