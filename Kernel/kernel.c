#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <MemoryManager.h>
#include <syscallManager.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


static const uint64_t PageSize = 0x1000;

static void *const uCodeModuleAddress = (void *)0x400000;
static void *const uDataModuleAddress = (void *)0x500000;


typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
    memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
    return (void *)((uint64_t)&endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
            - sizeof(uint64_t)                    // Begin at the top of the stack
            );
}


void fun1(uint64_t * num1, uint64_t * num2){
    *num2 = *num1 + 10;
}

void fun2(char * str){
    str[4] = ' ';
    str[5] = 'c';
    str[6] = 'o';
    str[7] = 'm';
    str[8] = 'o';
    str[9] = 0;
}

void testMemoryManager(){
    uint64_t * num1 = sys_malloc(sizeof(uint64_t));
    uint64_t * num2 = sys_malloc(sizeof(uint64_t));
    *num1 = 4;

    *num2 = 7;

    printBase(*num1, 10);
    printBase(*num2, 10);


    printBase(*num1, 10);
    printBase(*num2, 10);


    fun1(num1, num2);


    char * str = sys_malloc(sizeof(char) * 10);

    str[0] = 'H';
    str[1] = 'o';
    str[2] = 'l';
    str[3] = 'a';
    str[4] = 0;

    printString(str, RED);

    fun2(str);

    printString(str, BLUE);
    printString(str, BLUE);
    printString(str, BLUE);
    printString(str, BLUE);
    return ;

}


void *initializeKernelBinary() {

    void *moduleAddresses[] = {
        uCodeModuleAddress,
        uDataModuleAddress
        };

    void * endOfModules = loadModules(&endOfKernelBinary, moduleAddresses);

    void * startOfMem = (void *)(((uint8_t *) endOfModules + PageSize - (uint64_t) endOfModules % PageSize));

    //printBase((uint64_t) startOfMem, 16);

    MemoryManagerADT memoryManager = createMemoryManager(startOfMem, startOfMem + sizeof(MemoryManagerADT));


    clearBSS(&bss, &endOfKernel - &bss);


    return getStackBase();
}


int main() {
    load_idt();
    clearScreen();
    // testMemoryManager();

    ((EntryPoint) uCodeModuleAddress)();



    return 0;
}
