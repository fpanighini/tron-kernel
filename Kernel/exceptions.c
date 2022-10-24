#include <videoDriver.h>
#include <stdint.h>
#include <exceptions.h>

void loadMain(void);

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

void divideZeroException(){
    clearScreen();
    printString(0,0, ERROR0, 1, penColor );
    while(1){
        // Esperar un input de keyboard
    }
    loadMain();
}

void invalidOpcodeException(){
    clearScreen();
    printString(0,0, ERROR6, 1, penColor );
    while(1){
        // Esperar un input de keyboard
    }
    loadMain();
}





