/* uCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

//void shell(void);
void shell(int argc, char ** argv);

char *v = (char *)0xB8000 + 79 * 2;

void start_shell(void){
    while(1){
        // printf("Hello\n");
        shortSleep(1);
    }
}

int main() {
    sys_clearScreen();
    char * argv[] = {0};
    exec("shell", &shell, argv, 0);
    //exec("shell2", &start_shell, argv, 8);
    while(1){
        // printf("INIT\n");
        // INIT PROCESS
        shortSleep(1);
    }

    return 0;
}
