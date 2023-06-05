#include <wc.h>

int wc(int argc, char **argv)
{

    //char * c = malloc(2);

    int counter = 1;

    char c[2] = {1, 0};
    
    printf("\nENTER ASCII: %d \n", '\n');

    do
    {
        sys_read(STDIN, (char *)&c, 2);
        printf("\nLETTER: %c ASCII: %d \n", c[0], c[0]);

        if (c[0] == '\n')
            counter++;

    } while (c[0] != 4);

    printf("Cantidad de lineas del input: %d\n", counter);
    return 0;
}