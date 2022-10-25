#include <syscallManager.h>
#include <videoDriver.h>
#include <stdint.h>
#include <timer.h>

void ringBell();

uint64_t sys_setFontSize(uint32_t size) {
    setFontSize(size);
    return 0;
}

uint64_t sys_write(char *string, Color color) {
    printString(0,0, (uint8_t *)string, color);
    return 0;
}

uint64_t sys_read() {
    //what ...
    printString(0,0, (uint8_t *)"sys_read\n", white);
    return 0;
}

uint64_t sys_time() {
    printString(0,0, (uint8_t *)"Time ->\n", white);
    return getTime();
}

uint64_t sys_date() {
    printString(0,0, (uint8_t *)"Date -> \n", white);
    return getDate();
}

uint64_t sys_paintScreen() {
    clearScreen();
    return 0;
}

uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color) {
    printString(0,0, (uint8_t *)"sys_drawLine", white);
    fillrect((uint16_t) x, (uint16_t) y, (uint16_t) width, (uint16_t) height, color);
    return 0;
}

uint64_t sys_bell() {
    printString(0,0, (uint8_t *)"sys_bell\n", white);
    ringBell();
    return 0;
}

uint64_t sys_widthScr() {
    printString(0,0, (uint8_t *)"sys_widthScr\n", white);
    return getWidth();
}

uint64_t sys_heightScr() {
    printString(0,0, (uint8_t *)"sys_heightScr\n", white);
    return getHeight(); 
}

uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {sys_write, sys_read, sys_time ,sys_date , sys_paintScreen , sys_drawRectangle , sys_bell, sys_heightScr, sys_widthScr, sys_setFontSize};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r7, uint64_t r6, uint64_t rax) {
    printString(0,0, (uint8_t *)"sys_handler\n", white);
    if ((rax < sizeof(syscall_handlers)/sizeof(syscall_handlers[1])) && syscall_handlers[rax] != 0x00)
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
    return 0;
}

