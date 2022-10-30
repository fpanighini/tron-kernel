#include <videoDriver.h>
#include <stdint.h>
#include <exceptions.h>
#include <syscallManager.h>


void endException(uint8_t * str);
void printSavedRegisters(void);

extern uint64_t registers[REGISTER_NUM];

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
    printString("\nYOU WILL BE RETURNED TO TROLL SHELL IN TEN SECONDS\n\n",green);
    printSavedRegisters();
    sys_wait(10000);
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

void printSavedRegisters(){
    // long * regs = getSavedRegisters();
    char * regNames[] = REGISTER_NAMES;
    int i = 0;
    for (; i < REGISTER_NUM - 1 ; i++){
        printString(regNames[i],magenta);
        printString("\t\t\t\t\t\t",magenta);
        printBin(registers[i]);
        printString("\n",magenta);
    }
    printString(regNames[i],magenta);
    printString("\t\t\t",magenta);
    printBin(registers[i]);
    printString("\n",magenta);
}




