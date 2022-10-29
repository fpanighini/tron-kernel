GLOBAL cpuVendor
GLOBAL keyPressed
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
GLOBAL getDay
GLOBAL getMonth
GLOBAL getYear
GLOBAL ringBell

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


getSeconds:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x00
    out 70h, al
    in al, 71h
    sti

    ret


getMinutes:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x02
    out 70h, al
    in al, 71h
    sti

    ret


getHours:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x04
    out 70h, al
    in al, 71h
    sti
    ret

getDay:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x07
    out 70h, al
    in al, 71h
    sti

    ret

getMonth:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x08
    out 70h, al
    in al, 71h
    sti

    ret

getYear:
	cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x09
    out 70h, al
    in al, 71h
    sti
    
    ret

keyPressed:
    mov rax, 0
.inicio: 
    in al, 64h
    and al, 0x01
    je .inicio
    in al, 60h
    ret

ringBell:
    ret

screenshot:

    mov [registers+1*8], rax
    mov [registers+2*8], rbx
    mov [registers+3*8], rcx
    mov [registers+4*8], rdx
    mov [registers+5*8], rsi
    mov [registers+6*8], rdi
    mov [registers+7*8], rbp

    mov rax, rsp
    add rax, 160
    mov [registers+8*8], rax

    mov [registers+9*8], r8
    mov [registers+10*8], r9
    mov [registers+11*8], r10
    mov [registers+12*8], r11
    mov [registers+13*8], r12
    mov [registers+14*8], r13
    mov [registers+15*8], r14
    mov [registers+16*8], r15

    mov rax, [rsp+15*8]
    mov [registers], rax ;RIP
    ret

section .bss
    registers resq 18

;vbe_get_info_block:
;        mov ax, 0x4F00				; get VBE BIOS info
;	mov di, vbe_info_block
;        int 0x10
;
;vbe_info_structure:
;	.signature		db "VBE2"	; indicate support for VBE 2.0+
;	.table_data:		resb 512-4	; reserve space for the table below
;
