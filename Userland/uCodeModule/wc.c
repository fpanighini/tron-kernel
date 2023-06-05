#include <wc.h>

int wc(int argc, char **argv)
{
    char c[] = {0, 0, 0};
    int counter = 1;

    do
    {
        read(0, c, 1);
        if (c[0] == '\n')
            counter++;

    } while (c[0] != 0);

    printf("Cantidad de lineas del input: %d\n", counter);
    return 0;
}