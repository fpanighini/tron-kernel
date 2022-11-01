GLOBAL cpuVendor
GLOBAL keyPressed
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
GLOBAL getDay
GLOBAL getMonth
GLOBAL getYear
;GLOBAL ringBell
GLOBAL _speaker_tone
GLOBAL _speaker_off
EXTERN sys_wait

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


;ringBell:
;    mov ax, 2153
;    out 42h, al
;    mov al, ah
;    out  42h, al
;
;    in al, 61h
;    or al, 00000011b
;    out 61h, al
;    ret





;rdi - Note frequency, only 16 bit effective
_speaker_tone:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    mov cx, ax
    mov al, 182 ; 10 11 011 0 16 bit binary - mode 3 - lobyte/hibyte - channel 2 https://wiki.osdev.org/Programmable_Interval_Timer
    out 43h, al
    mov ax, cx ; Set up frequency
    out 42h, al
    mov al, ah
    out 42h, al
    in al, 61h ; Switch PC speaker on
    or al, 03h
    out 61h, al

    mov rdi, 1000
    call sys_wait

    in al, 61h ; Switch speaker off
    and al, 0FCh
    out 61h, al

    mov rsp, rbp
    pop rbp
    ret

_speaker_off:
    push rbp
    mov rbp, rsp

    in al, 61h
    and al, 0xFC
    out 61h, al

    mov rsp, rbp
    pop rbp
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
