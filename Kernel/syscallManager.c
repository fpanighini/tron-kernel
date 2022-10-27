#include <syscallManager.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <timer.h>

void ringBell();

uint64_t sys_setFontSize(uint32_t size) {
    setFontSize(size);
    return 0;
}

uint64_t sys_write(char *string, Color color) {
    // printString("sys_write\n", color);
    printString((uint8_t *)string, color);
    return 0;
}

uint64_t sys_read(uint8_t fd, char * buf, uint16_t count) {
    // printString("sys_read\n", white);
    if (fd != 0){
        return 0;
    }
    int i = 0;
    readBuf();
    while (i < count - 1){
        sys_wait(1);
        if (!keyRead()){
            buf[i] = readBuf();
            if (buf[i] == '\n' || buf[i] == 0){
                buf[i] = 0;
                return i;
            }
            i++;
        }
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_time() {
    printStringAt(0,0, (uint8_t *)"Time ->\n", white);
    return getTime();
}

uint64_t sys_date() {
    printStringAt(0,0, (uint8_t *)"Date -> \n", white);
    return getDate();
}

uint64_t sys_clearScreen() {
    clearScreen();
    return 0;
}

uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color) {
    printStringAt(0,0, (uint8_t *)"sys_drawLine", white);
    fillrect((uint16_t) x, (uint16_t) y, (uint16_t) width, (uint16_t) height, color);
    return 0;
}

uint64_t sys_bell() {
    printStringAt(0,0, (uint8_t *)"sys_bell\n", white);
    ringBell();
    return 0;
}

uint64_t sys_widthScr() {
    printStringAt(0,0, (uint8_t *)"sys_widthScr\n", white);
    return getWidth();
}

uint64_t sys_heightScr() {
    printStringAt(0,0, (uint8_t *)"sys_heightScr\n", white);
    return getHeight(); 
}

uint64_t sys_writeAtX(uint16_t x, char *string, Color color) {
    printStringAtX(x, (uint8_t *)string, color);
    return 0;
}

uint64_t sys_wait(uint16_t ticks){
    wait(ticks);
    return 0;
}

// uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {sys_write, sys_read, sys_time ,sys_date , sys_paintScreen , sys_drawRectangle , sys_bell, sys_heightScr, sys_widthScr, sys_setFontSize};
// 
// uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
//     // printDec(rax);
//     if ((rax < sizeof(syscall_handlers)/sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0x00){
//         // printString((uint8_t *)"sys_handler\n", white);
//         return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
//     }
//     return 0;
// }

