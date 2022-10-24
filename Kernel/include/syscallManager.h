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
static void syscallHandler();
static uint64_t sys_write();
static uint64_t sys_read();
static uint64_t sys_time();             // Tiempo hh:mm:ss
static uint64_t sys_date();             // dd/mm/aa
static uint64_t sys_paintScreen();      // 
static uint64_t sys_drawRectangle();
static uint64_t sys_bell();

#endif
