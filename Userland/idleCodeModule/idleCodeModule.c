/* uCodeModule.c */
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

void shell(void);

char *v = (char *)0xB8000 + 79 * 2;

int main() {
    while(1){
        shortSleep(1);
        printf("IDLE\n");
    }
    return 0;
}
