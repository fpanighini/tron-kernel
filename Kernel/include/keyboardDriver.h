#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_

#include <stdint.h>

void saveKey();

uint32_t readBuf(char * str, uint32_t count);

void clearKeyboardBuffer();

uint8_t getCount();

#endif



