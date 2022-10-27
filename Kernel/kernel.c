#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>
// include syscallmanager for testing
#include <syscallManager.h>

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


void getTimeFormat(uint8_t *buffer);
int itoa(int number, char *s);
void getDateFormat(uint8_t *buffer);

int main()
{
    load_idt();
    colorScreen(black);

    char string[] = "WELCOME TO THE TROLL KERNEL\nNo jodas";
    char newLine[] = "\n"; // only works with one
    char str[] = "new line\n";

    sys_write(string, white);
    sys_write(newLine, black);
    sys_clearScreen();
    sys_write(str, white);
    sys_write(string, white);
    sys_write(newLine, black);
    sys_clearScreen();
    uint8_t timeBuffer[100];
    getTimeFormat(timeBuffer);
    sys_write((char *)timeBuffer, green);
    sys_write(newLine, black);
    uint8_t dateBuffer[100];
    getDateFormat(dateBuffer);
    sys_write((char *)dateBuffer, green);
    sys_write(newLine, black);
    sys_setFontSize(2);
    int width = sys_widthScr(); 
    char h[3], w[3];
    itoa(width, w);
    sys_write(w, green);
    sys_write(newLine, black);
    int height = sys_heightScr();
    itoa(height, h);
    sys_write(h, gray);
    sys_setFontSize(1);
    sys_write("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n", white);
    sys_write("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n", white);
    sys_write(string, red);
    sys_write(newLine, white);
    sys_write("sdahcvsadkvjvdskkjñvnlnkfvwblkfhnfojhadbsnvlinjvhnldyhanliuvjbahlvkunjdyfhnlkjdfhblkgvjdnhvaubshvlkudubnasdkfjhailgvdfavdfhvpishvpoisbuenivbepofugpiuhnepognarpibgernhipobnheiupearnhiupaenhialgjy", blue);
    sys_write(newLine, white);
    clearScreen();
    sys_write("$ ", green);
    sys_write(newLine, white);

    sys_write("hla como estas\n", white);

    sys_write("como estas\n", white);

    //sys_bell();

    clearScreen();

    // char str1[10];
    // sys_read(0, (char *) &str1, 10);

    // sys_write(str1, green);

    // sys_read(0, (char *) &str1, 10);

    // sys_write(str1, green);
    int a = 0;

    a = ((EntryPoint) sampleCodeModuleAddress)();

    if (a == 1){
        sys_write("Hola", red);
    }
    else {
        sys_write("chaus", red);
    }
    while (1)
    {
    }

    return 0;
}

void getTimeFormat(uint8_t *buffer)
{
    char *p = (char *) buffer;
    int time = sys_time();
    int hours = ((time)&0xFF), minutes = ((time >> 8) & 0xFF), seconds = ((time >> 16) & 0xFF);
    if (hours > 3)
        hours -= 3;
    if (hours == 0)
        hours = 21;
    if (hours == 1)
        hours = 22;
    if (hours == 2)
        hours = 23;
    char sHours[3], sMinutes[3], sSeconds[3];
    itoa(hours, sHours);
    itoa(minutes, sMinutes);
    itoa(seconds, sSeconds);
    *p = sHours[0];
    p[1] = sHours[1]; 
    p[2] = ':';
    p[3] = sMinutes[0];
    p[4] = sMinutes[1];
    p[5] = ':';
    p[6] = sSeconds[0];
    p[7] = sSeconds[1];
    p[8] = 0;
}

void getDateFormat(uint8_t *buffer)
{
    char *p = (char *) buffer;
    uint64_t date = sys_date();
    itoa((((date)&0xFF) - 0x01), p); // -1 ??????? NighTime
    p[2] = '/';
    itoa((date >> 8) & 0xFF, &p[3]);
    p[5] = '/';
    itoa((date >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}

int itoa(int number, char *s) //UserSpace
{
    int digits = 1;
    for (int n = number / 10; n != 0; digits++, n /= 10)
        ;

    if (digits == 1)
    {
        s[0] = '0';
        s[1] = number + '0';
        s[2] = 0;
        return digits;
    }

    s[digits] = 0;
    for (int i = digits - 1; i >= 0; i--)
    {
        s[i] = (number % 10) + '0';
        number /= 10;
    }

    return digits;
}
