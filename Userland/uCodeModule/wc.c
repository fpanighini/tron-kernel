#include <wc.h>

int wc(int argc, char **argv)
{
    char c[3] = {0};
    char *buff = malloc(MAX_BUFFER_SIZE);
    int i = 0;

    do
    {
        read(STDIN, (char *)&c, 2);

        if (c[0] != 0 && c[0] != 4)
        {
            putChar(c[0]);
            buff[i++] = c[0];
        }

        if (c[1] != 0 && c[1] != 4)
        {
            putChar(c[1]);
            buff[i++] = c[1];
        }

    } while ((c[0] != 4 && c[0] != 0) || (c[1] != 4 && c[1] != 0));

    int charCount = 0;
    int wordCount = 0;
    int lineCount = 0;
    bool inWord = false;

    for (int i = 0; buff[i] != '\0'; i++)
    {
        charCount++;

        if (buff[i] == '\n')
            lineCount++;

        // Check for word boundary
        if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n')
            inWord = false;
        else if (!inWord)
        {
            inWord = true;
            wordCount++;
        }
    }

    // Increment line count if the last line didn't end with a newline
    if (charCount > 0 && buff[charCount - 1] != '\n')
        lineCount++;

    printf("\n %d    %d    %d\n", charCount, wordCount, lineCount);
    return 0;
}
