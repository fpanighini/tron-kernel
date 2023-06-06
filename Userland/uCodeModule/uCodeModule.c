// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* uCodeModule.c */
#include "include/lib.h"
#include "include/syscalls.h"
#include <color.h>
#include <timer.h>
#include <lib.h>

void shell(int argc, char **argv);

char *vvvvv = (char *)0xB8000 + 79 * 2;

int main()
{
    clear();
    yield();
    char *argv[] = {"Hola", 0};

    exec("shell", &shell, argv, 0, 1, 0, 1);
    while (1)
    {
        shortSleep(1);
    }

    return 0;
}
