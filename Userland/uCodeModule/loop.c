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
