BITS 64

    global asm_puts

    extern asm_strlen

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; size_t asm_puts(const char *str);
    ;; @rdi: str

asm_puts:

    push rbp                        ; Prologue
    mov rbp, rsp

    call asm_strlen

    push rsi                    ; Store values of $rsi and $rdx
    push rdx

    mov rdx, rax                ; Pass length
    mov rax, 1                  ; Write syscall
    mov rsi, rdi                ; Adress of buf
    mov rdi, 1                  ; File descriptor, STDOUT
    syscall

end:

    pop rdx                      ; Restore rdx and rsi registers
    pop rsi

    mov rsp, rbp                    ; Epilogue
    pop rbp

    ret
