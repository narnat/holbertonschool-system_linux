BITS 64

    global asm_strlen

    section .text

asm_strlen:

    push rbp
    mov rbp, rsi

    xor rax, rax

loop_strlen:

    cmp byte [rdi + rax], 0
    je end
    inc rax
    jmp loop_strlen

end:

    mov rsi, rbp
    pop rbp

    ret
