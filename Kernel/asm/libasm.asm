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


ringBell:
    ret


;vbe_get_info_block:
;        mov ax, 0x4F00				; get VBE BIOS info
;	mov di, vbe_info_block
;        int 0x10
;
;vbe_info_structure:
;	.signature		db "VBE2"	; indicate support for VBE 2.0+
;	.table_data:		resb 512-4	; reserve space for the table below
;
