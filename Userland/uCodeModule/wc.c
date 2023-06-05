#include <wc.h>

int wc(int argc, char **argv)
{
    int counter = 1;

    /*
    char * c = malloc(2);

    do
    {
        //printf("\nC0: %d \n", c[0]);
        //printf("\nCS: %s \n", c);

        if (c[0] == '\n')
            counter++;

    } while (c[0] != 0);

    //char c[2] = {1, 0};
    //(char *)&c, 2);
    */

    char ch;

    do
    {
        ch = getChar();

        printf("CHAR - %d - CHAR", ch);

        if(ch == '\n')
            counter++;

    } while (ch != 4);

    printf("Cantidad de lineas del input: %d\n", counter);
    return 0;
}