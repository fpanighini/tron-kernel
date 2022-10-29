GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL syscallINTHandler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher

EXTERN sys_write
EXTERN sys_read
EXTERN sys_writeAt
EXTERN sys_clearScreen
EXTERN sys_wait
EXTERN sys_time
EXTERN sys_date
EXTERN sys_heightScr
EXTERN sys_widthScr
EXTERN sys_timedRead
EXTERN sys_drawRectangle



SECTION .text

%macro pushState 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popState 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro irqHandlerMaster 1
    pushState

    mov rdi, %1 ; pasaje de parametro
    call irqDispatcher

    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al

    popState
    iretq
%endmacro



%macro exceptionHandler 1
    pushState

    mov rdi, %1 ; pasaje de parametro
    call exceptionDispatcher

    popState
    iretq
%endmacro


_hlt:
    sti
    hlt
    ret

_cli:
    cli
    ret


_sti:
	sti
	ret

picMasterMask:
    push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


; Zero Division Exception
 _exception0Handler:
 	exceptionHandler 0

; Invalid Opcode Exception
 _exception6Handler:
 	exceptionHandler 6

syscallINTHandler:
    cli
    mov rcx, r10
    mov r9, rax
    sti

    cmp rax, 0x00
    je .write

    cmp rax, 0x01
    je .read

    cmp rax, 0x02
    je .writeAt

    cmp rax, 0x03
    je .clearScreen

    cmp rax, 0x04
    je .wait

    cmp rax, 0x05
    je .time

    cmp rax, 0x06
    je .date

    cmp rax, 0x07
    je .getScreenHeight

    cmp rax, 0x08
    je .getScreenWidth

    cmp rax, 0x09
    je .timedRead

    cmp rax, 0x0A
    je .drawRectangle

    jmp .end

.write:
    call sys_write
    jmp .end

.read:
    call sys_read
    jmp .end

.writeAt:
    call sys_writeAt
    jmp .end
	
.clearScreen:
    call sys_clearScreen
    jmp .end

.wait:
    call sys_wait
    jmp .end

.time:
	call sys_time
	jmp .end
	
.date:
	call sys_date
	jmp .end

.getScreenHeight:
	call sys_heightScr
	jmp .end

.getScreenWidth:
	call sys_widthScr
	jmp .end

.timedRead:
	call sys_timedRead
	jmp .end

.drawRectangle:
	call sys_drawRectangle
	jmp .end

.end:
    iretq

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1

