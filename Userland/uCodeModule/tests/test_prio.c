#include "include/tests.h"
#include "test_util.h"

#define MINOR_WAIT 1000000
#define WAIT 100000000

#define TOTAL_PROCESSES 3


#define LOWEST 5
#define MEDIUM  3
#define HIGHEST 0


int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio()
{
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = exec("endless_loop_print", &endless_loop_print, argv, 0, 1, HIGHEST, 0);
    busy_wait(WAIT);
    printf("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        change_priority(pids[i], prio[i]);

    busy_wait(WAIT);
    printf("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        block(pids[i]);

    printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        change_priority(pids[i], MEDIUM);

    printf("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        unblock(pids[i]);

    busy_wait(WAIT);
    printf("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        kill(pids[i]);
}
