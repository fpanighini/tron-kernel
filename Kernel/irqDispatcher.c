#include <stdint.h>
#include <lib.h>
#include <videoDriver.h>



void irqDispatcher(uint64_t irq) {
    switch (irq) {
        case 0:
            int_20();
            break;
        case 1:
            int_21();
            break;
    }
    return;
}

void int_20() {
    timer_handler();
}

void int_21(){
    char a = keyPressed();
    if (a < 128 && a > 0){
        clearScreen();
        Color white = {0xff, 0x00, 0x00};
        printChar(0,0,getKey(a),4,white);
        printString(0,0,"\n",4,white);
    }
}



