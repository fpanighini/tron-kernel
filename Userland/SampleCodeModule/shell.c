#include <syscalls.h>
#include <color.h>
#include <time.h>
#include <lib.h>

#define COMMAND_CHAR "$> "
#define BACKSPACE '\b'
#define MAX_KBD_BUF 55

void shell();
void bufferRead(char **buf);
int readBuffer(char *buf);
void printLine(char *str);

void shell()
{
    char str[100];
    char *string = str;
    int out = 1;
    while (out)
    {
        bufferRead(&string);
        out = readBuffer(string);

        sys_write("\n", white);
        sys_write(string,blue);
        sys_write("\n",white);
    }
}

void bufferRead(char **buf)
{
    char c[2] = {1, 0};
    int i = 0;
    (*buf)[i] = 0;
    sys_write(COMMAND_CHAR, green);
    while (c[0] != 0)
    {
        sys_read(0, (char *)&c, 2);
        if (c[0] == BACKSPACE)
        {
            if (i > 0)
            {
                buf[i--] = 0;
                sys_write("\b", white);
            }
        }
        else
        {
            (*buf)[i++] = c[0];
            (*buf)[i] = 0;
            sys_write((*buf + i - 1), white);
        }
    }
}

int readBuffer(char *buf)
{

    return 1;
}

void printLine(char *str)
{
    sys_writeAtX(0, COMMAND_CHAR, green);
    sys_write(str, white);
}
