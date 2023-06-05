#include <wc.h>

int wc(int argc, char **argv)
{

    printf("LLEGUE WC\n");
    int counter = 0;
    char c[200] = {0};

    int i = 0;
    do
    {
        sys_read(STDIN, (char *)&c, 199);

        int aux = 0;
        while (c[aux] != 4 && c[aux] != 0)
            putChar(c[aux++]);

        printf("\nSTRING: %s \n", c);

        while (c[i] != 4 && c[i] != 0)
        {
            if (c[i++] == '\n')
            {
                counter++;
            }
        }

    } while (c[i] != 4 && c[i] != 0);

    printf("\nCantidad de lineas del input: %d\n", counter == 0 ? 1 : counter);
    return 0;
}
