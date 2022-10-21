
global getTime

getTime:

mov al, 0
mov 70h, al

mov rax, rdi
add rax, 70h
in al, rax


leave
ret
