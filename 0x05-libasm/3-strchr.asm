BITS 64

    global asm_strchr

    section .text

asm_strchr:

    push rbp                        ; prologue
    mov rbp, rsi

    xor rax, rax
    xor r8b, r8b

loop_strchr:

    mov r8b, BYTE [rdi]
    test r8b, r8b
    jz check_zero
    cmp r8b, sil
    je set_return
    inc rdi
    jmp loop_strchr

check_zero:

    test sil, sil
    jnz end

set_return:

    mov rax, rdi

end:

    mov rsi, rbp                    ; epilogue
    pop rbp

    ret
