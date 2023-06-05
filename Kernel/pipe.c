// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipe.h>

pipe_t pipes[MAX_PIPES];

void block_process_pipe(int *p, int pid);
void release_process_pipe(int *p, int pid);

// TODO: Ver este codigo

void fill0(char *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

int pipe_write(int index, char *addr, int n)
{

    //printString("EN PIPE WRITE1\n", MAGENTA);

    if (!pipes[index].created)
        return -1;
    block_process_pipe(pipes[index].wProcesses, get_running_pid());
    
    //printString("EN PIPE WRITE2\n", MAGENTA);
    
    sem_wait(pipes[index].name);
    
    //printString("EN PIPE WRITE3\n", MAGENTA);
    
    release_process_pipe(pipes[index].wProcesses, get_running_pid());
    int i;
    for (i = 0; i < n && addr[i] != 0 && (pipes[index].nread) != (pipes[index].nwrite + 1); i++)
    {
        pipes[index].data[pipes[index].nwrite++ % PIPE_SIZE] = addr[i];
        printString(pipes[index].data[pipes[index].nwrite-1 % PIPE_SIZE], MAGENTA);
    }
    //printString("EN PIPE WRITE4\n", MAGENTA);
    pipes[index].data[pipes[index].nwrite % PIPE_SIZE] = -1;
    sem_post(pipes[index].name);
    //printString("EN PIPE WRITE5\n", MAGENTA);
    return i;
}

int pipe_read(int index, char *addr, int n)
{
    printString(pipes[index].data, GREEN);

    if (!pipes[index].created)
        return -1;
    block_process_pipe(pipes[index].rProcesses, get_running_pid());
    sem_wait(pipes[index].name);
    release_process_pipe(pipes[index].wProcesses, get_running_pid());
    ///

    int i;
    for (i = 0; i < n && pipes[index].data[pipes[index].nread % PIPE_SIZE] != -1 && (pipes[index].nread + 1) != (pipes[index].nwrite); i++)
    {
        // print_char(pipes[index].data[pipes[index].nread % PIPE_SIZE]);
        addr[i] = pipes[index].data[pipes[index].nread++ % PIPE_SIZE];
        //printString(addr[i], MAGENTA);
    }
    if ((pipes[index].nread + 1) == (pipes[index].nwrite))
    {
        addr[i] = pipes[index].data[(pipes[index].nread) % PIPE_SIZE];
        addr[i + 1] = 0;
        pipes[index].nread++;
    }
    else
    {
        addr[i] = 0;
    }
    if (pipes[index].data[pipes[index].nread % PIPE_SIZE] == -1)
        pipes[index].data[pipes[index].nread % PIPE_SIZE] = 0;
    //_internal_print_string("post\n");
    sem_post(pipes[index].name);
    printString(addr, BLUE);
    return i;
}

int next_pipe()
{
    int aux = 0;
    while (aux < MAX_PIPES && pipes[aux].created != 0)
        aux++;
    return (aux == MAX_PIPES) ? -1 : aux;
}

int my_strcmp(const char *X, const char *Y)
{
    while (*X)
    {
        if (*X != *Y)
            break;
        X++;
        Y++;
    }
    return *(const unsigned char *)X - *(const unsigned char *)Y;
}

int look_pipe(char *name)
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        if (my_strcmp(name, pipes[i].name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int pipe_open(char *name)
{
    int i;
    if ((i = look_pipe(name)) != -1)
    {
        return pipes[i].fd;
    }
    int first_free = next_pipe();
    if (first_free == -1)
        return -1;
    pipes[first_free].fd = first_free + 3;
    pipes[first_free].created = 1;
    pipes[first_free].usingPipe = 0;
    pipes[first_free].waitingPid = -1;
    pipes[first_free].nread = 0;
    pipes[first_free].nwrite = 0;
    fill0(pipes[first_free].data, PIPE_SIZE);
    sem_open(name, 1);
    strcpy(pipes[first_free].name, name);
    return pipes[first_free].fd; // devuelvo el file descriptor de mi pipe que sera el que use mi proceso para acceder al buffer
}

void pipe_close(int index)
{
    sem_close(pipes[index].name);
    for (int i = 0; i < PROCS; i++)
    {
        pipes[index].rProcesses[i] = 0;
        pipes[index].wProcesses[i] = 0;
    }
    fill0(pipes[index].name, NAME_MAX);
    fill0(pipes[index].data, PIPE_SIZE);
    pipes[index].data[0] = 0;
    pipes[index].nread = 0;
    pipes[index].nwrite = 0;
    pipes[index].created = 0;
}

void list_blocked_processes(int *p, char *buf)
{
    char pid[5] = {0};
    for (int i = 0; i < PROCS; i++)
    {
        if (p[i] != 0)
        {
            itoa(p[i], pid);
            strcat(buf, pid);
            strcat(buf, ", ");
        }
    }
}

char retpipes[MAX_PIPES * 100] = {0};

char *pipes_info()
{
    fill0(retpipes, MAX_PIPES * 100);
    int cant = 0;
    char namePipe[10], brp[50] = {0}, bwp[50] = {0};
    for (int p = 0; p < MAX_PIPES; p++)
    {
        if (pipes[p].created != 0)
        {
            cant++;
            // itoa(p, namePipe, 10);
            strcpy(namePipe, pipes[p].name);
            list_blocked_processes(pipes[p].rProcesses, brp);
            list_blocked_processes(pipes[p].wProcesses, bwp);
            strcat(retpipes, "Pipe: ");
            strcat(retpipes, namePipe);
            strcat(retpipes, "\n");
            strcat(retpipes, "State: ");
            strcat(retpipes, pipes[p].data + (pipes[p].nread % PIPE_SIZE));
            strcat(retpipes, "\n");
            strcat(retpipes, "Blocked Read Processes: ");
            strcat(retpipes, brp);
            strcat(retpipes, "\n");
            strcat(retpipes, "Blocked Write Processes: ");
            strcat(retpipes, bwp);
            strcat(retpipes, "\n");
        }
    }
    if (cant == 0)
        strcpy(retpipes, "No pipes available\n");
    return retpipes;
}

void block_process_pipe(int *p, int pid)
{
    int i;
    for (i = 0; i < PROCS && p[i] != 0; i++)
    {
        if (p[i] == pid)
        {
            block_process(pid);
            return;
        }
    }
    if (i == PROCS)
        return;
    p[i] = pid;
    block_process(pid);
}

void release_process_pipe(int *p, int pid)
{
    for (int i = 0; i < PROCS; i++)
    {
        if (p[i] == pid)
        {
            ready_process(p[i]);
            p[i] = 0;
            return;
        }
    }
}
