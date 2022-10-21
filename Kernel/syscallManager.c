#include <syscallManager.h>
#include <stdint.h>
#include <videoDriver.h>

Color penColor = {0x7F, 0x7F, 0x7F};

void syscallHandler() {
    printString(0,0, "sys_handler", 2, penColor);

}

uint64_t sys_write() {
    printString(0,0, "sys_write", 2, penColor);

}

uint64_t sys_read() {
    printString(0,0, "sys_read", 2, penColor);

}

uint64_t sys_time() {
    printString(0,0, "sys_time", 2, penColor);
    return get_time();
}

uint64_t sys_date() {
    printString(0,0, "sys_date", 2, penColor);
    return get_date();
}

void sys_clearScreen() {
    clearScreen();
}

uint64_t sys_drawLine() {
    printString(0,0, "sys_drawLine", 2, penColor);

}

uint64_t sys_bell() {
    printString(0,0, "sys_bell", 2, penColor);

}
