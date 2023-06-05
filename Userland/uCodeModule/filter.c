#include "include/filter.h"

void filter(int argc, char **argv)
{

    char *buff = malloc(MAX_BUFFER_SIZE);

    for (int i = 0; i < MAX_BUFFER_SIZE; i++)
        buff[i] = 0;

    int cant = read(0, buff, MAX_BUFFER_SIZE);

    int j = 0;
    for (int i = 0; i < cant || buff[i] == 0; i++)
        if (isVowel(buff[i]))
            buff[j++] = buff[i];

    buff[j] = '\0';
    printf("\nFiltered string: %s\n", buff);
}

int isVowel(char c)
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}