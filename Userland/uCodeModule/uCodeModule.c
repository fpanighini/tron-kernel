/* uCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

void shell(void);

char *v = (char *)0xB8000 + 79 * 2;

void start_shell(void){
    shell();
}

int main() {
    sys_clearScreen();
    exec("shell", &start_shell);
    while(1){
        // INIT PROCESS
        shortSleep(1);
    }

    return 0;
}
