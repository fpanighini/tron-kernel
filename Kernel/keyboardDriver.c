// BORRAR:
#include <videoDriver.h>

#include <stdint.h>
#include <keyboardDriver.h>
#include <interrupts.h>

#define BUF_SIZE 55

extern uint8_t keyPressed(void);

uint8_t getKey(uint8_t id);

typedef struct buf {
    uint8_t keys[BUF_SIZE];
    //uint8_t read;
    uint8_t count;
} bufT;

bufT buf = {0,0};

void saveKey(){
    uint8_t c = keyPressed();
    if (c > 128){
        return ;
    }
    buf.keys[buf.count++] = getKey(c);
    //buf.keys[buf.count] = 0;
    //printString(buf.keys,blue);
}

uint32_t readBuf(char * str, uint32_t count){
    _cli();
    int i = 0;
    while (i < buf.count && i < count){
        str[i] = buf.keys[i];
        i++;
    }
    clearKeyboardBuffer();
    _sti();
    return i;
}

void clearKeyboardBuffer(){
    buf.count = 0;
}

uint8_t getCount(){
    return buf.count;
}

// uint8_t keyRead(){
//     return buf.read;
// }

uint8_t getKey(uint8_t id)
{
    if (id >= 128)
    {
        return -1;
    }
    char kbd_US[128] =
        {
            0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
            '\t', /* <-- Tab */
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
            0, /* <-- control key */
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
            '*',
            0,   /* Alt */
            ' ', /* Space bar */
            0,   /* Caps lock */
            0,   /* 59 - F1 key ... > */
            0, 0, 0, 0, 0, 0, 0, 0,
            0, /* < ... F10 */
            0, /* 69 - Num lock*/
            0, /* Scroll Lock */
            0, /* Home key */
            0, /* Up Arrow */
            0, /* Page Up */
            '-',
            0, /* Left Arrow */
            0,
            0, /* Right Arrow */
            '+',
            0, /* 79 - End key*/
            0, /* Down Arrow */
            0, /* Page Down */
            0, /* Insert Key */
            0, /* Delete Key */
            0, 0, 0,
            0, /* F11 Key */
            0, /* F12 Key */
            0, /* All other keys are undefined */
        };

    return kbd_US[id];
}


