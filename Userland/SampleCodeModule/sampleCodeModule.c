/* sampleCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

// BORRAR:
#include <tron.h>
#include <lib.h>
//

void shell(void);

char *v = (char *)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;

int main()
{
    sys_clearScreen();
    shell();
    return 0;
}
