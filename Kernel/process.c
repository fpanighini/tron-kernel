// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/process.h"
#include "include/pipe.h"
#include <syscallManager.h>
#include "include/scheduler.h"
#include <scheduler.h>
#include <lib.h>
#include <semaphore.h>

uint64_t pidc = 0;

char **save_argv(int argc, char **argv);
uint64_t count_argv(char **argv);
void free_argv(char **argv);

ProcessP newProcess(char *name, void *entryPoint, char **argv, uint64_t read_fd, uint64_t write_fd, uint64_t priority)
{
    ProcessP proc = malloc(sizeof(Process));
    if (proc == NULL)
        return NULL;

    int argc = count_argv(argv);

    char **saved_argv = save_argv(argc, argv);

    void *stack = malloc(STACK_FRAME_SIZE);
    if (stack == NULL) {
        free(proc);
        return NULL;
    }

    char *stackBase = (char *)stack + STACK_FRAME_SIZE - sizeof(uint64_t);

    StackFrame stackFrame = createStack(entryPoint, stackBase, argc, saved_argv);

    memcpy((char *)stack + STACK_FRAME_SIZE - sizeof(StackFrame), &stackFrame, sizeof(StackFrame));

    proc->name = name;
    proc->argv = saved_argv;

    sem_wait(PIDC_MUTEX);
    proc->pid = pidc++;
    sem_post(PIDC_MUTEX);

    proc->ppid = get_running_pid();
    proc->sp = (uint64_t)stack + STACK_FRAME_SIZE - sizeof(StackFrame);
    proc->bp = (uint64_t)stackBase;
    proc->state = NEW;
    proc->stack = stack;
    proc->read_fd = read_fd;
    proc->write_fd = write_fd;
    proc->priority = priority;
    proc->children = 0;
    proc->blocked_by_children = 0;

    return proc;
}

void wait_pid()
{
    ProcessP proc = get_current_proc();
    if (proc->children != 0)
    {
        proc->blocked_by_children = 1;
        block_current_process();
    }
}

void free_proc(ProcessP proc)
{
    free(proc->stack);
    free_argv(proc->argv);
    char *eof = (char *)4;
    sys_write(proc->write_fd, eof, 1, WHITE);
    free(proc);
}

void free_argv(char **argv)
{
    uint64_t i = 0;
    while (argv[i] != NULL)
    {
        free(argv[i++]);
    }
    free(argv);
}

char **save_argv(int argc, char **argv)
{
    char **ret = malloc((argc + 1) * sizeof(char *));
    if (ret == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < argc; i++)
    {
        ret[i] = malloc(strlen(argv[i]) + 1); // +1 for the null terminator
        if (ret[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(ret[j]);
            free(ret);
            return NULL;
        }
        strcpy(ret[i], argv[i]);
    }

    ret[argc] = NULL;
    return ret;
}

uint64_t count_argv(char **argv)
{
    uint64_t count = 0;
    while (argv[count] != NULL)
    {
        count++;
    }
    return count;
}
