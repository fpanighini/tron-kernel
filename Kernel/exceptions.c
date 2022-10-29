#include <videoDriver.h>
#include <stdint.h>
#include <exceptions.h>

void endException(uint8_t * str);


void exceptionDispatcher(uint64_t e){
    switch (e) {
        case 0:
            divideZeroException();
            break;
        case 6:
            invalidOpcodeException();
            break;
    }
    return;
}

void endException(uint8_t * errorString){
    colorScreen(blue);
    printStringAt(0,0, (uint8_t *) errorString, red);
    printString("\nYOU WILL BE RETURNED TO TROLL SHELL IN FIVE SECONDS",green);
    sys_wait(5000);
    loadMain();
}

void divideZeroException(){
    clearScreen();
    endException(ERROR0);
}

void invalidOpcodeException(){
    clearScreen();
    endException(ERROR6);
}





