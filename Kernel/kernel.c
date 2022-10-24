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

int getSeconds();
int getMinutes();
int getHours();
int getDay();
int getMonth();
int getYear();

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

void getTimeFormat(uint8_t* buffer);
int itoa(int number, char* s);
void getDate(uint8_t* buffer);

int main()
{
    load_idt();
    colorScreen(black);

    uint8_t string[] = "WELCOME TO THE TROLL KERNEL\nNo jodas";
    uint8_t newLine[] = "\n"; // only works with one
    uint8_t str[] = "new line\n";

    printString(0, 0, string, 2, white);
    printString(0, 0, newLine, 2, green);
    clearScreen();
    printString(0, 0, str, 2, gray);
    printString(0, 0, string, 2, gray);
    printString(0, 0, newLine, 2, gray);
    testInt(); // overlap -> solution increment heigh or reduce font size
    clearScreen();
    uint8_t timeBuffer[100];
    getTimeFormat(timeBuffer);
    printString(0, 0, timeBuffer, 2, white);
    printString(0, 0, newLine, 2, green);
    uint8_t dateBuffer[100];
    getDate(dateBuffer);
    printString(0, 0, dateBuffer, 2, green);

    while (1)
    {
    }

    return 0;
}

void getTimeFormat(uint8_t* buffer) {
	char* p = (char *) buffer;
    int hours = getHours();
    if(hours>3)
        hours -=3;
    if(hours==0)
        hours=21;
    if(hours==1)
        hours=22;
    if(hours==2)
        hours=23;
    int minutes = getMinutes();
    int seconds = getSeconds();
    char sHours[3], sMinutes[3], sSeconds[3];
    itoa(hours, sHours);
    itoa(minutes, sMinutes);
    itoa(seconds, sSeconds);
    *(p)=sHours[0];
    *(p+1)=sHours[1];
    *(p+2)=':';
    *(p+3)=sMinutes[0];
    *(p+4)=sMinutes[1];
    *(p+5)=':';
    *(p+6)=sSeconds[0];
    *(p+7)=sSeconds[1];
    *(p+8)=0;
}

void getDate(uint8_t* buffer) {
	char* p = (char *) buffer;
	uint64_t date = getDay() | ((uint64_t)getMonth() << 8) | ((uint64_t)getYear() << 16);
	itoa((date) & 0xFF, p);
	p[2] = '/';
	itoa((date >> 8) & 0xFF, &p[3]);
	p[5] = '/';
	itoa((date >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}


int itoa(int number, char* s) {
    int digits = 1;
	for (int n=number/10; n != 0; digits++, n /= 10);

    if(digits == 1) {
        s[0] = '0';
        s[1] = number + '0';
        s[2] = 0;
        return digits;
    }
	
	s[digits] = 0;    
	for (int i=digits-1; i>=0; i--) {
		s[i] = (number % 10) + '0';
		number /= 10;
	}
	
	return digits;
}
