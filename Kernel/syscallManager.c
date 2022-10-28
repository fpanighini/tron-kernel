#include <syscallManager.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <timer.h>

#define WAIT_TIME 10

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

uint64_t sys_read(uint8_t fd, char * buf, uint32_t count) {
    //printString("sys_read\n", white);
    if (fd != 0){
        return 0;
    }
    int i = 0;
    clearKeyboardBuffer();
    while (i < count - 1){
        // printString("paso 1 segundo",red);
        //_hlt();
        if (getCount()){
            i += readBuf(buf + i, count - i - 1);
            clearKeyboardBuffer();
            if (buf[i-1] == '\n' || buf[i-1] == 0){
                buf[i-1] = 0;
                return i;
            }
        }
        // sys_wait(WAIT_TIME);
        _hlt();
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_time() {
    //printStringAt(0,0, (uint8_t *)"Time ->\n", white);
    //printString("Time Kernel:\n", red);
    //printDec(getTime());
    //printString("\n", black);
    //printString("End KErnel\n",red);
    return getTime();
}

uint64_t sys_date() {
    //printStringAt(0,0, (uint8_t *)"Date -> \n", white);
    //printString("Date KErnel:\n",red);
    //printDec(getDate());
    //printString("\nend KErnel date\n", red);
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

uint64_t sys_writeAt(uint32_t x, uint32_t y, char *string, Color color) {
    printStringAt(x, y, (uint8_t *)string, color);
    return 0;
}

uint64_t sys_wait(uint32_t millis){
    int initial = milliseconds_elapsed();
    while (milliseconds_elapsed() - initial < millis){
        _hlt();
    }
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

