/* sampleCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>

void shell(void);

char *v = (char *)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;

int main()
{
    // sys_write("\n[Userland]\n", green);

    // char str[10];
    // char * string = str;

    // sys_read(0, string,2);

<<<<<<< HEAD
    shell();
    while (1)
    {
        sys_wait(1);
    }
=======
    // sys_write(string, red);
    // sys_write("\n",red);
    // sys_write("hola\bhola\n\n",red);
    sys_clearScreen();

    shell();
>>>>>>> 53bde04 (Shell interpreter implemented)

    return 100;
}
