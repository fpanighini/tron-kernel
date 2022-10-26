#include <stdint.h>
#include <keyboardDriver.h>

extern uint8_t keyPressed(void);

uint8_t getKey(uint8_t id);

typedef struct buf {
    uint8_t key;
    uint8_t read;
} bufT;

bufT buf = {0,0};

void saveKey(){
    uint8_t c = keyPressed();
    if (c > 128){
        return ;
    }
    buf.key = c;
    buf.read = 0;
}

uint8_t readBuf(){
    buf.read = 1;
    return getKey(buf.key);
}

uint8_t keyRead(){
    return buf.read;
}

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


