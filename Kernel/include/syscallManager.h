#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_

/*
    Delete Comments!!
    Lists of irq's
    Check return type.
*/
void syscallHandler();
uint64_t sys_write();
uint64_t sys_read();
uint64_t sys_time();
uint64_t sys_date();
void sys_clearScreen();
uint64_t sys_drawLine();
uint64_t sys_bell();

#endif