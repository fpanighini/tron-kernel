#include <cat.h>

void cat(int argc, char *args[])
{
    char *buff = malloc(MAX_BUFFER_SIZE);

    for (int i = 0; i < MAX_BUFFER_SIZE; i++)
    {
        buff[i] = 0;
    }

    inputRead(&buff);

    printf("\n%s\n", buff);
}
