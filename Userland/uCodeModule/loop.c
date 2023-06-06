// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <loop.h>

int loop(int argc, char **argv)
{
    while (1)
    {
        shortSleep(1000);
        printf("My PID: %d\n", get_pid());
    }
    return 0;
}
