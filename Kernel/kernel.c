// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <memoryManager.h>
#include <syscallManager.h>
#include <scheduler.h>

#define HEAP_SIZE 0x100000

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

void clearBSS(void *bssAddress, uint64_t bssSize)
{
    memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
    return (void *)((uint64_t)&endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
                    - sizeof(uint64_t)                    // Begin at the top of the stack
    );
}

void *initializeKernelBinary()
{

    void *moduleAddresses[] = {
        uCodeModuleAddress,
        uDataModuleAddress};

    void *endOfModules = loadModules(&endOfKernelBinary, moduleAddresses);

    clearBSS(&bss, &endOfKernel - &bss);

    void *startOfMem = (void *)((uint8_t *)endOfModules + PageSize - (uint64_t)endOfModules % PageSize);
    initMemoryManager(startOfMem, HEAP_SIZE);
    init_scheduler();

    return getStackBase();
}

int main()
{
    load_idt();
    clearScreen();

    ((EntryPoint)uCodeModuleAddress)();

    return 0;
}
