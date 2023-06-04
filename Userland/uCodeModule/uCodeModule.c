/* uCodeModule.c */
#include "include/lib.h"
#include "include/syscalls.h"
#include <syscalls.h>
#include <color.h>
#include <timer.h>
#include <lib.h>

//void shell(void);
void shell(int argc, char ** argv);

void procA();
void procB();

char *v = (char *)0xB8000 + 79 * 2;

void start_shell(int argc, char ** argv){
     for (int i = 0 ; i < argc ; i++){
       printf("%d %s\n",i, argv[i]);
    }
    while(1){
        // printf("Hello\n");
        shortSleep(1);
    }
}

//TODO: Eliminar esto
void procA(){
    sem_wait("semA");
    while (1){
        sys_write(1, "HOLA", 5, WHITE);
        sem_post("semB");
        sem_wait("semA");
    }
}

void procB(){
    sem_post("semA");
    while (1){
        sem_wait("semB");
        char buf[100];
        sys_read(0, buf, 10);
        printf("YO SOY B Y LEI: %s\n", buf);
        sem_post("semA");
    }
}


int main() {
    sys_clearScreen();
    sys_yield();
    char * argv[] = {"Hola", 0};



    // Consumer y producer
    // sem_open("semA", 0);
    // sem_open("semB", 0);

    // int fd = pipe_open("PROCS");

    // exec("procA", &procA, argv, 0, fd, 0);
    // exec("procB", &procB, argv, fd, 1, 0);



    exec("shell", &shell, argv, 0, 1, 0);
    //exec("shell2", &start_shell, argv, 8);
    while(1){
        // printf("INIT\n");
        // INIT PROCESS
        shortSleep(1);
    }

    return 0;
}
