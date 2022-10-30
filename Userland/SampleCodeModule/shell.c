#include <syscalls.h>
#include <color.h>
#include <timer.h>
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
#define DIVIDE_BY_ZERO "div-zero"
#define INVALID_OP "invalid-op"
#define DATE_COMMAND "date"
#define TIME_COMMAND "time"
#define DATE_TIME_COMMAND "datetime"
#define INC_FONT_SIZE_COMMAND "inc-font"
#define DEC_FONT_SIZE_COMMAND "dec-font"
#define INFOREG_COMMAND "inforeg"

#define MAX_TERMINAL_CHARS 124          // 124 = (1024/8) - 4 (number of characters that fit in one line minus the commmand prompt and cursor characters)
#define HELP_MESSAGE "HELP"

#define INCREASE 1
#define DECREASE -1

#define REGISTER_NUM 18
#define REGISTER_NAMES {"RIP", "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "rFlags"}

#define COMMAND_NOT_FOUND_MESSAGE "Command not found"
#define INCREASE_FONT_FAIL "Font upper size limit reached"
#define DECREASE_FONT_FAIL "Font lower size limit reached"

#define NEWLINE "\n"

void shell();
void bufferRead(char **buf);
int readBuffer(char *buf);
void printLine(char *str);
void helpCommand(void);
void printNewline(void);
void testDivideByZeroExcpetion();
void testInvalidOpException();

void printInforeg();

void increaseFontSize(int currentSize);
void decreaseFontSize(int currentSize);

extern void invalidOpcode();
extern void divideZero();

void shell()
{
    char str[100];
    char *string = str;
    int out = 1;

    while (out)
    {
        bufferRead(&string);
        printf("\b");
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
    }
    else if (!strcmp(buf, HELP_COMMAND)){
        helpCommand();
    }
    else if (!strcmp(buf, CLEAR_COMMAND)){
        clear();
    }
    else if (!strcmp(buf, TRON_COMMAND)){
        clear();
        mainTron();
        clear();
    }
    else if (!strcmp(buf, DATE_COMMAND)){
        char str[MAX_TERMINAL_CHARS] = {0};
        char * string = str;
        getDateFormat(string);
        printf("%s\n",string);
    }
    else if (!strcmp(buf, TIME_COMMAND)){
        char str[MAX_TERMINAL_CHARS] = {0};
        char * string = str;
        getTimeFormat(string);
        printf("%s\n",string);
    }
    else if (!strcmp(buf, DATE_TIME_COMMAND)){
        char str[MAX_TERMINAL_CHARS] = {0};
        char * string = str;
        getDateAndTime(string);
        printf("%s\n",string);
    }
    else if (!strcmp(buf, INC_FONT_SIZE_COMMAND)){
        int check = sys_changeFontSize(INCREASE);
        if (!check){
            printErrorMessage(buf, INCREASE_FONT_FAIL);
            printNewline();
        }
        else {
            clear();
        }
    }
    else if (!strcmp(buf, DEC_FONT_SIZE_COMMAND)){
        int check = sys_changeFontSize(DECREASE);
        if (!check){
            printErrorMessage(buf, DECREASE_FONT_FAIL);
            printNewline();
        }
        else {
            clear();
        }
    }
    else if (!strcmp(buf, INFOREG_COMMAND)){
        printInforeg();
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
        sys_clearScreen();
        return 0;
    }
    else {
        printErrorMessage(buf, COMMAND_NOT_FOUND_MESSAGE);
        printNewline();
    }
    return 1;
}

void printErrorMessage(char * program, char * errorMessage){
    sys_write(SHELL_NAME, green);
    sys_write(": ", green);
    sys_write(program, white);
    sys_write(": ", green);
    sys_write(errorMessage, red);
}

void helpCommand(){
    sys_write(HELP_MESSAGE,white);
    printNewline();
}

void printNewline(){
    sys_write(NEWLINE,white);
}

void testDivideByZeroExcpetion(){
    divideZero();
}


void testInvalidOpException(){
    invalidOpcode();
}

void printInforeg(){
    long array[REGISTER_NUM] = {0};
    long * arr = &array;
    int check = sys_inforeg(arr);
    char * registerNames[] = REGISTER_NAMES;
    if (check){
        for (int i = 0 ; i < REGISTER_NUM; i++){
            printf("%s : ",registerNames[i]);
            printBase(arr[i], 16);
            printf("h\n");
        }

    } else {
        printf("First you need to press a snapshot key. Try CTRL!");
    }
}
