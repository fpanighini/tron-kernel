#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

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
        sampleCodeModuleAddress,
        sampleDataModuleAddress};

    loadModules(&endOfKernelBinary, moduleAddresses);

    clearBSS(&bss, &endOfKernel - &bss);

    return getStackBase();
}

void testInt(void); // -> interrupts.asm

int main()
{
    load_idt();
    Color backColor = {0x00, 0x00, 0x00};
    colorScreen(backColor);

    Color penColor = {0xFF, 0xFF, 0xFF};
    uint8_t string[] = "WELCOME TO THE TROLL KERNEL\nNo jodas";
    uint8_t newLine[] = "\n"; // only works with one
    uint8_t str[] = "new line\n";

    printString(0, 0, string, 2, penColor);
    printString(0, 0, newLine, 2, penColor);
    clearScreen();
    printString(0, 0, str, 2, penColor);
    printString(0, 0, string, 2, penColor);
    printString(0, 0, newLine, 2, penColor);
    testInt(); // overlap -> solution increment heigh or reduce font size

    while (1)
    {
    }

    return 0;
}