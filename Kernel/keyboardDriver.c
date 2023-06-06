#include "include/scheduler.h"
#include "include/videoDriver.h"
#include <stdint.h>
#include <stdbool.h>
#include <keyboardDriver.h>
#include <interrupts.h>
#include <videoDriver.h>

#define BUF_SIZE 64
#define CTRL 31

extern uint8_t keyPressed(void);

uint8_t getKey(uint8_t id);

typedef struct CircBuffer
{
    char keys[BUF_SIZE];
    size_t read;
    size_t write;
    size_t size;
} CircBuffer;

static CircBuffer buffer = {{0}, 0, 0, 0};
static bool shift = false;
static bool ctrl = false;

void push(char c) {
    if (buffer.size < BUF_SIZE) {
        buffer.keys[buffer.write] = c;
        buffer.write++;
        buffer.size++;
        if (buffer.write == BUF_SIZE)
            buffer.write = 0;
    }
}

int pop() {
    int c = -1;
    if (buffer.size > 0) {
        c = buffer.keys[buffer.read];
        buffer.read++;
        buffer.size--;
        if (buffer.read == BUF_SIZE)
            buffer.read = 0;
    }
    return c;
}

void saveKey(uint8_t scan_code)
{
    // clearScreen();
    // printBase(c, 16);
    if (scan_code == 0b111000){
        return;
    }
    if (scan_code == 1)
    {
        push('\n');
        return;
    }
    if (scan_code == 0x2A || scan_code == 0x36)
    {
        shift = 1;
        return;
    }
    if (scan_code == 0xAA || scan_code == 0xB6)
    {
        shift = 0;
        return;
    }

    if (scan_code == 0xE0 || scan_code == 0x0)
    {
        //printString("CTRL OFF\n",WHITE);
        ctrl = 0;
        return;
    }

    if (scan_code == 0x1D)
    {
        // printString("CTRL ON\n", WHITE);
        ctrl = 1;
        return;
    }
    if (shift)
    {
        if (scan_code == 0x8)
        {
            push('&');
            return;
        }
        if (scan_code == 0x2B)
        {
            push('|');
            return;
        }

    }
    if (ctrl) {
        if (getKey(scan_code) == 'c') {
            kill_foreground_proc();
            ready_foreground_proc();
            ctrl = 0;
            return ;
        }
        if (getKey(scan_code) == 'd') {
            push(4);
            ctrl = 0;
            return ;
        }
        //return ;
    }
    if (scan_code > 128)
        return;
    push(getKey(scan_code) + ('A' - 'a') * shift);
}

uint32_t readBuf(char *str, uint32_t count)
{
    _cli();
    int i;
    for (i = 0; i < count; i++) {
        int c = pop();
        if (c == -1) break;
        str[i] = c;
    }
    _sti();
    return i;
}

uint8_t getCount()
{
    return buffer.size;
}

uint8_t getKey(uint8_t id)
{
    if (id >= 128)
        return -1;

    char kbd_US[128] =
        {
            0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
            '\t', /* <-- Tab */
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
            5, /* <-- control key */
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '^', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '^',
            '*',
            '^', /* Alt */
            ' ', /* Space bar */
            '^', /* Caps lock */
            '^', /* 59 - F1 key ... > */
            '^', '^', '^', '^', '^', '^', '^', '^',
            '^', /* < ... F10 */
            '^', /* 69 - Num lock*/
            '^', /* Scroll Lock */
            '^', /* Home key */
            3,   /* Up Arrow */
            '^', /* Page Up */
            '-',
            1, /* Left Arrow */
            '^',
            2, /* Right Arrow */
            '+',
            '^', /* 79 - End key*/
            4,   /* Down Arrow */
            '^', /* Page Down */
            '^', /* Insert Key */
            '^', /* Delete Key */
            '^', '^', '^',
            '^', /* F11 Key */
            '^', /* F12 Key */
            '^', /* All other keys are undefined */
        };

    return kbd_US[id];
}
