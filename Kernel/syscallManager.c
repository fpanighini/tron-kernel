#include <syscallManager.h>

#define WAIT_TIME 10

void ringBell();

int getKbdBuffer(char * buf, uint32_t count, int * pos);

extern uint64_t registers[REGISTER_NUM];


uint64_t sys_write(uint8_t fd, char *string, Color color) {
    if (fd == STDERR){
        color = RED;
    }
    printString((uint8_t *)string, color);
    return 0;
}


int getKbdBuffer(char * buf, uint32_t count, int * pos){
    if (getCount()){
        *pos += readBuf(buf + *pos, count - *pos - 1);
        clearKeyboardBuffer();
        return 1;
    }
    return 0;
}

uint64_t sys_read(uint8_t fd, char * buf, uint32_t count) {
    if (fd != 0){
        return 0;
    }
    int i = 0;
    int read = 0;
    clearKeyboardBuffer();
    while (i < count - 1){
        read = getKbdBuffer(buf, count, &i);
        if (read && (buf[i-1] == '\n' || buf[i-1] == 0)){
            buf[i-1] = 0;
            return i;
        }
        _hlt();
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_timedRead(uint8_t fd, char * buf, uint32_t count, uint32_t millis) {
    if (fd != STDIN){
        return 0;
    }
    int i = 0;
    int read = 0;
    clearKeyboardBuffer();
    buf[0] = 0;
    int initial = milliseconds_elapsed();
    while ((i < count - 1) && ((milliseconds_elapsed() - initial) < millis)){
        read = getKbdBuffer(buf, count, &i);
        if (read && (buf[i-1] == '\n' || buf[i-1] == 0)){
            buf[i-1] = 0;
            return i;
        }
        _hlt();
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_time() {
    return getTime();
}

uint64_t sys_date() {
    return getDate();
}

uint64_t sys_clearScreen() {
    clearScreen();
    return 0;
}

uint64_t sys_drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    fillrect((uint16_t) x, (uint16_t) y, (uint16_t) width, (uint16_t) height, color);
    return 0;
}

uint64_t sys_bell() {
    printStringAt(0,0, (uint8_t *)"sys_bell\n", WHITE);
    ringBell();
    return 0;
}

uint64_t sys_widthScr() {
    return getWidth();
}

uint64_t sys_heightScr() {
    return getHeight(); 
}

uint64_t sys_writeAt(uint16_t x, uint16_t y, char *string, Color color) {
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


uint64_t sys_inforeg(uint64_t array[REGISTER_NUM]){
    if (registers != 0){
        for (int i = 0 ; i < REGISTER_NUM ; i++){
            array[i] = registers[i];
        }
        return 1;
    }
    return 0;
}


uint64_t sys_changeFontSize(uint32_t size) {
    return changeFontSize(size);
}

