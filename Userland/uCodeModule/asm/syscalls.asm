GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_writeAt
GLOBAL sys_clearScreen
GLOBAL sys_wait
GLOBAL sys_time
GLOBAL sys_date
GLOBAL sys_getScreenHeight
GLOBAL sys_getScreenWidth
GLOBAL sys_timedRead
GLOBAL sys_drawRectangle
GLOBAL sys_changeFontSize
GLOBAL sys_inforeg
GLOBAL sys_beep
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_exec
GLOBAL sys_pipe_open
GLOBAL sys_pipes_info
GLOBAL sys_pipe_close
GLOBAL sys_sem_open
GLOBAL sys_sem_close
GLOBAL sys_sem_post
GLOBAL sys_sem_wait
GLOBAL sys_sem_info
GLOBAL sys_sem_count
GLOBAL sys_get_pid
GLOBAL sys_kill
GLOBAL sys_block
GLOBAL sys_unblock
GLOBAL sys_yield
GLOBAL sys_change_priority
GLOBAL sys_ps
GLOBAL sys_wait_pid

; syscall 0x00
sys_write:
    mov rax, 0x00
    mov r10, rcx
    int 0x80
    ret

; syscall 0x01
sys_read:
    mov rax, 0x01
    int 0x80
    ret

; syscall 0x02
sys_writeAt:
    mov rax, 0x02
    mov r10, rcx
    int 0x80
    ret

; syscall 0x03
sys_clearScreen:
    mov rax, 0x03
    int 0x80
    ret

; syscall 0x04
sys_wait:
    mov rax, 0x04
    int 0x80
    ret

; syscall 0x05
sys_time:
    mov rax, 0x05
    int 0x80
    ret

; syscall 0x06
sys_date:
    mov rax, 0x06
    int 0x80
    ret

; syscall 0x07
sys_getScreenHeight:
    mov rax, 0x07
    int 0x80
    ret

; syscall 0x08
sys_getScreenWidth:
    mov rax, 0x08
    int 0x80
    ret

; syscall 0x09
sys_timedRead:
    mov rax, 0x09
    mov r10, rcx
    int 0x80
    ret

; syscall 0x0A
sys_drawRectangle:
    mov rax, 0x0A
    mov r10, rcx
    int 0x80
    ret

; syscall 0x0B
sys_changeFontSize:
    mov rax, 0x0B
    int 0x80
    ret

; syscall 0x0C
sys_inforeg:
    mov rax, 0x0C
    mov r10, rcx
    int 0x80
    ret

; syscall 0x0D
sys_beep:
    mov rax, 0x0D
    int 0x80
    ret

; syscall 0x0E
sys_malloc:
    mov rax, 0x0E
    int 0x80
    ret

sys_free:
    mov rax, 0x0F
    int 0x80
    ret

; syscall 0x10
sys_exec:
    mov rax, 0x10
    int 0x80
    ret

sys_pipe_open:
    mov rax, 0x11
    mov r10, rcx
    int 0x80
    ret

sys_pipes_info:
    mov rax, 0x12
    mov r10, rcx
    int 0x80
    ret

sys_pipe_close:
    mov rax, 0x13
    mov r10, rcx
    int 0x80
    ret

sys_sem_open:
    mov rax, 0x14
    mov r10, rcx
    int 0x80
    ret

sys_sem_close:
    mov rax, 0x15
    mov r10, rcx
    int 0x80
    ret

sys_sem_post:
    mov rax, 0x16
    mov r10, rcx
    int 0x80
    ret

sys_sem_wait:
    mov rax, 0x17
    mov r10, rcx
    int 0x80
    ret

sys_sem_info:
    mov rax, 0x18
    mov r10, rcx
    int 0x80
    ret

sys_sem_count:
    mov rax, 0x19
    mov r10, rcx
    int 0x80
    ret

sys_get_pid:
    mov rax, 0x1A
    mov r10, rcx
    int 0x80
    ret

sys_kill:
    mov rax, 0x1B
    mov r10, rcx
    int 0x80
    ret

sys_block:
    mov rax, 0x1C
    mov r10, rcx
    int 0x80
    ret

sys_unblock:
    mov rax, 0x1D
    mov r10, rcx
    int 0x80
    ret

sys_yield:
    mov rax, 0x1E
    mov r10, rcx
    int 0x80
    ret

sys_change_priority:
    mov rax, 0x1F
    mov r10, rcx
    int 0x80
    ret

sys_ps:
    mov rax, 0x20
    mov r10, rcx
    int 0x80
    ret

sys_wait_pid:
    mov rax, 0x21
    mov r10, rcx
    int 0x80
    ret


