#include "include/timer.h"
#include "include/lib.h"

void getDateFormat(char *buffer);
void getTimeFormat(char *buffer);

/**
 * @brief Get the Current Date
 *
 * @return char*
 */
void getDateAndTime(char *buff)
{
    char *p = buff;
    getDateFormat(p);
    p[8] = ' ';
    getTimeFormat(&p[9]);
}

void getTimeFormat(char *buff)
{
    char *p = buff;
    long timeVar = time();
    int hours = ((timeVar)&0xFF), minutes = ((timeVar >> 8) & 0xFF), seconds = ((timeVar >> 16) & 0xFF);
    if (hours > 3)
        hours -= 3;
    if (hours == 0)
        hours = 21;
    if (hours == 1)
        hours = 22;
    if (hours == 2)
        hours = 23;
    itoa(hours, p);
    p[2] = ':';
    itoa(minutes, &p[3]);
    p[5] = ':';
    itoa(seconds, &p[6]);
    p[8] = 0;
}

void getDateFormat(char *buff)
{
    char *p = buff;
    long dateVar = date();
    itoa((dateVar & 0xFF), p);
    p[2] = '/';
    itoa((dateVar >> 8) & 0xFF, &p[3]);
    p[5] = '/';
    itoa((dateVar >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}
