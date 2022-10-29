#include <syscalls.h>
#include <color.h>
#include <time.h>
#include <lib.h>
#include <tron.h>

#define COMMAND_CHAR "$> "
#define CURSOR "|"
#define BACKSPACE '\b'
#define MAX_KBD_BUF 55

#define SHELL_NAME "Troll"

#define HELP_COMMAND "help"
#define CLEAR_COMMAND "clear"
#define EXIT_COMMAND "exit"
#define TRON_COMMAND "tron"
#define COMMAND_NOT_FOUND_MESSAGE "Command not found"
#define DIVIDE_BY_ZERO "div-zero"
#define INVALID_OP "invalid-op"


#define HELP_MESSAGE "HELP"

#define NEWLINE "\n"

void shell();
void bufferRead(char **buf);
int readBuffer(char *buf);
void printLine(char *str);
void helpCommand(void);
void printNewline(void);
void testDivideByZeroExcpetion();
void testInvalidOpException();

extern invalidOpcode();

void shell()
{
    char str[100];
    char *string = str;
    int out = 1;
    while (out)
    {
        bufferRead(&string);
        printNewline();
        out = readBuffer(string);
    }
}

void bufferRead(char **buf)
{
    int c = 1;
    int i = 0;
    (*buf)[i] = 0;
    sys_write(COMMAND_CHAR, green);
    sys_write(CURSOR, white);
    while (c != 0)
    {
        c = getChar();
        if (c == BACKSPACE)
        {
            if (i > 0)
            {
                buf[i--] = 0;

                sys_write("\b", white);
                sys_write("\b", white);
                sys_write(CURSOR, white);
            }
        }
        else if (c >= ' ')
        {
            (*buf)[i++] = c;
            (*buf)[i] = 0;

            sys_write("\b", white);
            sys_write((*buf + i - 1), white);
            sys_write(CURSOR, white);
        }
    }
}

int readBuffer(char *buf)
{
    if (!strcmp(buf, "")){
        return 1;
    }
    else if (!strcmp(buf, HELP_COMMAND)){
        helpCommand();
        return 1;
    }
    else if (!strcmp(buf, CLEAR_COMMAND)){
        sys_clearScreen();
        return 1;
    }
    else if (!strcmp(buf, TRON_COMMAND)){
        sys_clearScreen();
        mainTron();
        sys_clearScreen();
        return 1;
    }
    else if (!strcmp(buf, DIVIDE_BY_ZERO)){
        testDivideByZeroExcpetion();
        return 0;
    }
    else if (!strcmp(buf, INVALID_OP)){
        testInvalidOpException();
        return 0;
    }
    else if (!strcmp(buf, EXIT_COMMAND)){
        return 0;
    }
    else {
        sys_write(SHELL_NAME, green);
        sys_write(": ", green);
        sys_write(buf, white);
        sys_write(": ", green);
        sys_write(COMMAND_NOT_FOUND_MESSAGE, red);
        printNewline();
        return 1;
    }
    return 1;
}

void helpCommand(){
    sys_write(HELP_MESSAGE,white);
    printNewline();
}

void printNewline(){
    sys_write(NEWLINE,white);
}

void testDivideByZeroExcpetion(){
    int i = 1/0;
}


void testInvalidOpException(){
    invalidOpcode();
}
