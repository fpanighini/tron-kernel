GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_writeAtX    ; <- Borrar
GLOBAL sys_clearScreen
GLOBAL sys_wait
GLOBAL sys_writeAt
GLOBAL sys_time
GLOBAL sys_date

; syscall 0
sys_write:
    mov rax, 0x00
    int 0x80
    ret

; syscall 1
sys_read:
    mov rax, 0x01
    int 0x80
    ret

; syscall 2
sys_writeAtX:
    mov rax, 0x02
    int 0x80
    ret

; syscall 3
sys_clearScreen:
    mov rax, 0x03
    int 0x80
    ret

; syscall 4
sys_wait:
    mov rax, 0x04
    int 0x80
    ret

; syscall 5
sys_time:
    mov rax, 0x05
    int 0x80
    ret

; syscall 6
sys_date:
    mov rax, 0x06
    int 0x80
    ret

