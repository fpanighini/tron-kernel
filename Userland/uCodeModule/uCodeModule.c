/* uCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

void shell(void);

char *v = (char *)0xB8000 + 79 * 2;

void start_shell(void){
    while(1){
        printf("Hello\n");
        shortSleep(1);
    }
}

int main() {
    sys_clearScreen();
    exec("shell", &shell);
    while(1){
        // printf("INIT\n");
        // INIT PROCESS
        shortSleep(1);
    }

    return 0;
}
