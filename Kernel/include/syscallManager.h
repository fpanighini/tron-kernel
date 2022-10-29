#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_

#include <timer.h>
#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <interrupts.h>


#define SYSCALL_NUMBER 7



/*
    Delete Comments!!
    Lists of irq's
    Check return type.
*/
void syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax);
uint64_t sys_write(char *string, Color color);
uint64_t sys_read(uint8_t fd, char * buf, uint32_t count);
uint64_t sys_writeAt(uint16_t x, uint16_t y, char *string, Color color);
uint64_t sys_time();             // Tiempo hh:mm:ss
uint64_t sys_date();             // dd/mm/aa
uint64_t sys_clearScreen();      // clean screen
uint64_t sys_drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
uint64_t sys_bell();
uint64_t sys_widthScr();
uint64_t sys_heightScr();
uint64_t sys_setFontSize(uint32_t size); //put in syshandler
uint64_t sys_wait(uint32_t millis);
uint64_t sys_timedRead(uint8_t fd, char * buf, uint32_t count, uint32_t millis);

uint64_t sys_screenshot(void);
uint64_t sys_inforeg(uint64_t * array);

#endif
