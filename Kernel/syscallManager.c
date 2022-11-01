#include <syscallManager.h>

#define WAIT_TIME 10

void ringBell();

int getKbdBuffer(char * buf, uint32_t count, int * pos);

extern uint64_t registers[REGISTER_NUM];


uint64_t sys_write(char *string, Color color) {
    // printString("sys_write\n", color);
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
    //printString("sys_read\n", white);
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
    if (fd != 0){
        return 0;
    }
    int i = 0;
    int read = 0;
    clearKeyboardBuffer();
    buf[0] = 0;
    int initial = milliseconds_elapsed();
    //sys_wait(millis);
    while ((i < count - 1) && ((milliseconds_elapsed() - initial) < millis)){
        read = getKbdBuffer(buf, count, &i);
        if (read && (buf[i-1] == '\n' || buf[i-1] == 0)){
            buf[i-1] = 0;
            return i;
        }
        _hlt();
        // sys_wait(WAIT_TIME);
        //_hlt(); cual es la funcion de este hlt, no estaba ni incluida la bliblioteca de referencia
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

uint64_t sys_drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    // printStringAt(0,0, (uint8_t *)"sys_drawLine", white);
    fillrect((uint16_t) x, (uint16_t) y, (uint16_t) width, (uint16_t) height, color);
    return 0;
}

uint64_t sys_bell() {
    printStringAt(0,0, (uint8_t *)"sys_bell\n", white);
    //ringBell();
    return 0;
}

uint64_t sys_widthScr() {
    // printStringAt(0,0, (uint8_t *)"sys_widthScr\n", white);
    return getWidth();
}

uint64_t sys_heightScr() {
    // printStringAt(0,0, (uint8_t *)"sys_heightScr\n", white);
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
    //long * registers = getSavedRegisters();
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

void printTest(){
    sys_write("CTRL", red);
}
