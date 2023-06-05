#include <wc.h>

int wc(int argc, char **argv)
{
    int counter = 0;
    char c[3] = {0};

    do
    {
        read(STDIN, (char *)&c, 2);

        if(c[0] != 0 && c[0] != 4)
            putChar(c[0]);

        if(c[1] != 0 && c[1] != 4)
            putChar(c[1]);

        if (c[0] == '\n' || c[1] == '\n')
            counter = counter + 1;

    } while ((c[0] != 4 && c[0] != 0) || (c[1] != 4 && c[1] != 0));

    printf("\nCantidad de lineas del input: %d\n", counter == 0 ? 0 : counter+1);
    return 0;
}
