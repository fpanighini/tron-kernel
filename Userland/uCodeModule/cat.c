#include <cat.h>
//#define CURSOR "|"
//#define BACKSPACE '\b'

void cat(int argc, char *args[]) {

    //sem_open("cat", 1);


	char * buff = malloc(MAX_BUFFER_SIZE);

    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        buff[i] = 0;
    }

    printf("IN CAT\n");

    //sem_wait("cat");
	int cant = sys_read(0, buff, MAX_BUFFER_SIZE);

    printf("CAT length: %d\n", cant);
    
    for(int i = 0; buff[i]!=0 && i<cant; i++){
            printf("%c", buff[i]);
    }
    //sem_post("cat");
}