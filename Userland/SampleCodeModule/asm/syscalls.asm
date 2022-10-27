GLOBAL sys_read
GLOBAL sys_write

sys_write:
    mov rax, 0x00
    int 0x80
    ret

sys_read:
    mov rax, 0x01
    int 0x80
    ret


