/* sampleCodeModule.c */
#include <syscalls.h>
#include <color.h>

void shell(void);

char * v = (char*)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;


int main() {
    ////All the following code may be removed 
    //*v = 'X';
    //*(v+1) = 0x74;

    ////Test if BSS is properly set up
    //if (var1 == 0 && var2 == 0)
    //    return 0xDEADC0DE;
    //
    //
    sys_write("\n[Userland]\n", green);


    char str[10];
    char * string = str;

    sys_read(0, string,2);

    sys_write(string, red);
    sys_write("\n",red);
    sys_write("hola\bhola\n\n",red);


    shell();
    while(1){

    }


    return 100;
}
