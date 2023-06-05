#include <wc.h>

int wc(int argc, char **argv)
{
    int counter = 1;
    char c[2] = {1, 0};

    do
    {
        sys_read(STDIN, (char *)&c, 2);
        putChar(c[0]);

        //if(c[0] != 0)
        //    printf("\nLETTER: %c ASCII: %d \n", c[0], c[0]);

        if (c[0] == '\n')
            counter++;

    } while (c[0] != 4 && c[0] != 0);

    //printf("\nCantidad de lineas del input: %d\n", counter);
    return 0;
}