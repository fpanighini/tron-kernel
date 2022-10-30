#include <stdint.h>
#include <lib.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

void irqDispatcher(uint64_t irq) {
    /*
    switch (irq) {
        case 0:
            int_20();
            break;
        case 1:
            int_21();
            break;
    }
    return;
    */
}

void int_20() {
    //printString("tic\n",red);
    timer_handler();
}

void int_21(){
    char c = 0;
    saveKey(c);
}



