#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#define STANDARD_WAIT 5

void timerHandler();

int ticks_elapsed();

int seconds_elapsed();

uint64_t getTime();

uint64_t getDate();

void wait(uint16_t ticks);

#endif
