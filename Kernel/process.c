#include "include/scheduler.h"
#include <process.h>
#include <scheduler.h>
#include <lib.h>

uint64_t pidc = 0;

char ** save_argv(int argc, char ** argv);
uint64_t count_argv(char ** argv);
void free_argv(char ** argv);

ProcessP newProcess(char * name, void * entryPoint, char ** argv, uint64_t priority) {
    ProcessP proc = malloc(sizeof(Process));

    int argc =  count_argv(argv);

    char ** saved_argv = save_argv(argc, argv);

    void * stack = malloc(STACK_FRAME_SIZE);

    char * stackBase = stack + STACK_FRAME_SIZE - sizeof(uint64_t);

    StackFrame stackFrame = createStack(entryPoint, stackBase, argc, saved_argv);

    memcpy(stack + STACK_FRAME_SIZE - sizeof(StackFrame), &stackFrame, sizeof(StackFrame));

    proc->name = name;
    proc->argv = saved_argv;
    proc->pid = pidc++;
    proc->ppid = get_running_pid();
    proc->sp = (uint64_t) stack + STACK_FRAME_SIZE - sizeof(StackFrame);
    proc->bp = (uint64_t) stackBase;
    proc->state = NEW;
    proc->stack = stack;
    proc->priority = priority;
    return proc;
}

void free_proc(ProcessP proc){
    free(proc->stack);
    free_argv(proc->argv);
    free(proc);
}

void free_argv(char ** argv){
    uint64_t i = 0;
    while (argv[i] != NULL){
        free(argv[i++]);
    }
    free(argv);
}

char ** save_argv(int argc, char ** argv){
    char ** ret = malloc((argc + 1) * sizeof(char *));
    if (ret == NULL){
        return NULL;
    }
    for(int i = 0; i < argc; i++) {
        ret[i] = malloc(strlen(argv[i]) + 1); // +1 for the null terminator
        if(ret[i] == NULL) {
            // handle error, probably you should free the previously allocated memory here
            return NULL;
        }
        strcpy(ret[i], argv[i]);
    }

    ret[argc] = NULL; //
    return ret;
}

uint64_t count_argv(char ** argv){
    uint64_t count = 0;
    while(argv[count] != NULL){
        count++;
    }
    return count;
}


