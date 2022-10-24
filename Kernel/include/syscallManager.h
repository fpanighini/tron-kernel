#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_


#include <stdint.h>
#include <videoDriver.h>


#define SYSCALL_NUMBER 7



/*
    Delete Comments!!
    Lists of irq's
    Check return type.
*/
void syscallHandler();
uint64_t sys_write();
uint64_t sys_read();
uint64_t sys_time();             // Tiempo hh:mm:ss
uint64_t sys_date();             // dd/mm/aa
uint64_t sys_paintScreen();      // 
uint64_t sys_drawRectangle();
uint64_t sys_bell();
uint64_t sys_widthScr();
ui

#endif
