BITS 64

    global asm_putc

    section .text

asm_putc:

    push rbp                        ; prologue
    mov rbp, rsi

    push rsi
    push rdx

    mov rax, 1                  ; write syscall
    mov [rsi], rdi              ; Character
    mov rdi, 1                  ; stdout
    mov rdx, 1                  ; one byte
    syscall

end:

    pop rdx
    pop rsi

    mov rsi, rbp                    ; epilogue
    pop rbp

    ret
