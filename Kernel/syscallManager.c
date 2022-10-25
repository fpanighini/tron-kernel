#include <syscallManager.h>
#include <videoDriver.h>
#include <stdint.h>
#include <timer.h>

uint64_t sys_write(char *string, int size, Color color) {
    printString(0,0, (uint8_t *)string, (uint16_t) size, color);
    return 0;
}

uint64_t sys_read() {
    //what ...
    printString(0,0, (uint8_t *)"sys_read\n", 1, white);
    return 0;
}

uint64_t sys_time() {
    printString(0,0, (uint8_t *)"Time ->\n", 1, white);
    return getTime();
}

uint64_t sys_date() {
    printString(0,0, (uint8_t *)"Date -> \n", 1, white);
    return getDate();
}

uint64_t sys_paintScreen() {
    clearScreen();
    return 0;
}

uint64_t sys_drawRectangle(int x, int y, int size, Color color) {
    printString(0,0, (uint8_t *)"sys_drawLine\n", 1, white);
    printSquare((uint16_t) x, (uint16_t) y, (uint16_t) size, color);
    return 0;
}

uint64_t sys_bell() {
    printString(0,0, (uint8_t *)"sys_bell\n", 1, white);
    return 0;
}

uint64_t sys_widthScr() {
    printString(0,0, (uint8_t *)"sys_widthScr\n", 1, white);
    return getWidth; //warning ...
}

uint64_t sys_heightScr() {
    printString(0,0, (uint8_t *)"sys_heightScr\n", 1, white);
    return getHeight(); 
}

//check r7 and r6
uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r7, uint64_t r6) = {sys_write, sys_read, sys_time ,sys_date , sys_paintScreen , sys_drawRectangle , sys_bell, sys_heightScr, sys_widthScr};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r7, uint64_t r6, uint64_t rax) {
    printString(0,0, (uint8_t *)"sys_handler\n", 1, white);
    if ((rax < sizeof(syscall_handlers)/sizeof(syscall_handlers[1])) && syscall_handlers[rax] != 0x00)
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8, r7, r6);
    return 0;
}

