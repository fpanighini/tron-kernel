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
    // sys_write("\n[Userland]\n", green);

    sys_clearScreen();

    shell();

    //printf("Hola %s %d %x\n %s", "que tal", 17, 17, "abajo");


    //sys_write("\n[USERLAND]",white);

    //char str[10];
    //char * string = str;

    //sys_timedRead(0,string,10,1000);

    //shell();

    //sys_write(str,red);

//    char str[] = {0,0};
//    int c = 0;
//    while (c != '\n'){
//        c = getChar();
//        str[0] = c;
//        sys_write(str, red);
//    }

    //mainTron();


    return 0;
}
