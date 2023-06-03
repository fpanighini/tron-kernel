#include <tests.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc)
{
    printf("INSIDE slowInc (test_sync.c 11)\n");
    uint64_t aux = *p;
    yield(); // This makes the race condition highly probable
    aux += inc;
    *p = aux;
    printf("p value in slowInc: %d\n", *p);

}

uint64_t my_process_inc(uint64_t argc, char *argv[])
{

    printf("INSIDE my_process_inc (test_sync.c 20)\n");

    uint64_t n;
    int8_t inc;
    int8_t use_sem;

    if (argc != 3)
        return -1;

    if ((n = satoi(argv[0])) <= 0)
        return -1;
    if ((inc = satoi(argv[1])) == 0)
        return -1;
    if ((use_sem = satoi(argv[2])) < 0)
        return -1;

    printf("inc value in my_process_inc: %d\n", inc);

    if (use_sem)
        if (sem_open(SEM_ID, 1) < 0)
        {
            printf("test_sync: ERROR opening semaphore\n");
            return -1;
        }

    uint64_t i;
    for (i = 0; i < n; i++)
    {
        printf("INSIDE FOR (test_sync.c 46)\n");
        if (use_sem)
            sem_wait(SEM_ID);
        slowInc(&global, inc);
        if (use_sem)
            sem_post(SEM_ID);
    }

    if (use_sem)
        sem_close(SEM_ID);

    return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[])
{ //{n, use_sem, 0}
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

    if (argc != 2)
        return -1;

    char *argvDec[] = {argv[0], "-1", argv[1], NULL};
    char *argvInc[] = {argv[0], "1", argv[1], NULL};

    global = 0;

    uint64_t i;
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        pids[i] = exec("my_process_inc", &my_process_inc, argvDec, 0, 1, 3);
        pids[i + TOTAL_PAIR_PROCESSES] = exec("my_process_inc", &my_process_inc, argvInc, 0, 1, 3);
    }

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        shortSleep(pids[i]);
        shortSleep(pids[i + TOTAL_PAIR_PROCESSES]);
    }

    printf("Final value: %d\n", global);

    return 0;
}
