#include <stdint.h>
#include <lib.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

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
    saveKey();
    if (!keyRead()){
        char a = readBuf();
        if (a == -1){
            return ;
        }
        clearScreen();
        printChar(0,0,a,2,white);
        printString(0,0,(uint8_t *)"\n",2,white);
    }
}



