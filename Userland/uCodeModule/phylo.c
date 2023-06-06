#include <phylo.h>

#define MAX 10
#define MIN 2
#define INITIAL 5

#define LEFT (((id) + (shared_data->count) - 1) % (shared_data->count))
#define RIGHT (((id) + 1) % (shared_data->count))

#define MUTEX "phylo/main"
#define SEM_PHYLO_H "phylo/t/0"
#define SEM_PHYLO_T "phylo/h/0"

#define SEM_LEN 10

typedef enum State { THINKING, HUNGRY, EATING, LEAVING } State;

typedef struct SharedData {
    size_t count;
    State state[MAX];
    char hungry_lock[MAX][SEM_LEN];
    char think_unlock[MAX][SEM_LEN];
} SharedData;

void test(SharedData *shared_data, size_t id) {
    if (shared_data->state[id] == HUNGRY && shared_data->state[LEFT] != EATING &&
        shared_data->state[RIGHT] != EATING) {
        shared_data->state[id] = EATING;
        sys_sem_post(shared_data->hungry_lock[id]);
    }
}

int take_fork(SharedData *shared_data, size_t id) {
    sys_sem_wait(MUTEX);
    shared_data->state[id] = HUNGRY;
    test(shared_data, id);
    sys_sem_post(MUTEX);

    sys_sem_wait(shared_data->hungry_lock[id]);
}

void put_fork(SharedData *shared_data, int id) {
    sys_sem_wait(MUTEX);
    shared_data->state[id] = THINKING;

    test(shared_data, LEFT);
    test(shared_data, RIGHT);

    sys_sem_post(MUTEX);
}

int phylo_proc(int argc, char const *argv[]) {
    size_t id = atoi(argv[0]);
    SharedData *shared_data = (SharedData *)atoi(argv[1]);

    while (1) {
        take_fork(shared_data, id);
        sys_yield();
        put_fork(shared_data, id);

        sys_sem_post(shared_data->think_unlock[id]);
        sys_yield();
        sys_sem_wait(shared_data->think_unlock[id]);

        sem_wait(MUTEX);
        if (shared_data->state[id] == LEAVING) {
            sys_sem_post(MUTEX);
            sys_kill(sys_get_pid());
        }
        sys_sem_post(MUTEX);
    }
}

void addPhylo(SharedData *shared_data) {
    size_t id = shared_data->count;
    char id_str[] = "X";
    char addr[64];
    char *argv_proc[3] = {id_str, addr, NULL};
    id_str[0] = '0' + id;
    itoa(shared_data, addr);

    sys_sem_wait(MUTEX);
    shared_data->state[id] = THINKING;
    shared_data->count++;
    sys_exec("phylo_proc", &phylo_proc, argv_proc, STDIN, STDOUT, 1, 0);
    sys_sem_post(MUTEX);
}

void removePhylo(SharedData *shared_data) {
    size_t id = shared_data->count - 1;

    sys_sem_wait(shared_data->think_unlock[id]);
    sys_sem_wait(MUTEX);
    shared_data->state[id] = LEAVING;
    shared_data->count--;
    sys_sem_post(MUTEX);
    sys_sem_post(shared_data->think_unlock[id]);
}

int phylo(int argc, char *argv[]) {
    SharedData shared_data;
    shared_data.count = INITIAL;
    sys_sem_open(MUTEX, 0);

    size_t i;
    for (i = 0; i < MAX; i++) {
        strcpy(shared_data.hungry_lock[i], SEM_PHYLO_H);
        strcpy(shared_data.think_unlock[i], SEM_PHYLO_T);

        shared_data.hungry_lock[i][SEM_LEN - 2] = '0' + i;
        shared_data.think_unlock[i][SEM_LEN - 2] = '0' + i;

        sys_sem_open(shared_data.hungry_lock[i], 0);
        sys_sem_open(shared_data.think_unlock[i], 0);
    }
    char id[] = "X";
    char addr[64];
    char *argv_proc[3] = {id, addr, NULL};

    for (i = 0; i < shared_data.count; i++) {
        id[0] = '0' + i;
        itoa(&shared_data, addr);
        sys_exec("phylo_proc", &phylo_proc, argv_proc, STDIN, STDOUT, 1, 0);
    }

    sys_sem_post(MUTEX);

    while (1) {
        int c;
        while (c = getTimedChar(), c != '\0') {
            if (c == 'a' && shared_data.count < MAX) {
                addPhylo(&shared_data);
                break;
            } else if (c == 'r' && shared_data.count > MIN) {
                removePhylo(&shared_data);
                break;
            }
        }

        sys_sem_wait(MUTEX);
        char state[MAX+1];
        int i;
        for (i = 0; i < shared_data.count; i++)
            state[i] = shared_data.state[i] == EATING ? 'E' : '.';
        state[i] = '\n';
        state[i+1] = '\0';
        sys_write(STDOUT, state, i+1, WHITE);
        sys_sem_post(MUTEX);
        sys_yield();
    }
}
