#include <time.h>
#include <lib.h>
#include <syscalls.h>

void getDateFormat(char *buffer);
void getTimeFormat(char *buffer);

/**
 * @brief Get the Current Date
 *
 * @return char*
 */
void getCurrentDate()
{
    char date[9], time[9];
    getDateFormat(date);
    getTimeFormat(time);

    sys_write("Date: ", green);
    sys_write(date, white);
    //sys_write(" ", black);
    //sys_write("Time: ", green);
    //sys_write(time, white);
    //sys_write("\n", black);
}

void getTimeFormat(char *buffer)
{
    char *p = (char *)buffer;
    long time = sys_time();
    char test[20];
    atoi(time, test);
    sys_write(test, white);
    sys_write("\n", black);
    int hours = ((time)&0xFF), minutes = ((time >> 8) & 0xFF), seconds = ((time >> 16) & 0xFF);
    if (hours > 3)
        hours -= 3;
    if (hours == 0)
        hours = 21;
    if (hours == 1)
        hours = 22;
    if (hours == 2)
        hours = 23;
    char sHours[3], sMinutes[3], sSeconds[3];
    atoi(hours, sHours);
    atoi(minutes, sMinutes);
    atoi(seconds, sSeconds);
    *p = sHours[0];
    p[1] = sHours[1];
    p[2] = ':';
    p[3] = sMinutes[0];
    p[4] = sMinutes[1];
    p[5] = ':';
    p[6] = sSeconds[0];
    p[7] = sSeconds[1];
    p[8] = 0;
}
void getDateFormat(char *buffer)
{
    char *p = (char *)buffer;
    int date = sys_date();
    char test[20];
    atoi(date, test);
    sys_write(test, white);
    sys_write("\n", black);
    atoi((((date)&0xFF) - 0x01), p);
    p[2] = '/';
    atoi((date >> 8) & 0xFF, &p[3]);
    p[5] = '/';
    atoi((date >> 16) & 0xFF, &p[6]);
    p[8] = 0;
}
