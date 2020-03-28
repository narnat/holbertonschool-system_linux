BITS 64

    global asm_memcpy

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; void *asm_memcpy(void *dest, const void *src, size_t n);
    ;; @rdi: dest
    ;; @rsi: src
    ;; @rdx: n

asm_memcpy:

    push rbp                        ; Prologue
    mov rbp, rsp

    push r8                    ; Save values of r8 and r9
    push r9

    mov rax, rdi                ; Return value

    test rdi, rdi               ; Check if @dest or @src is NULL,  or @rdx is 0
    jz end
    test rsi, rsi
    jz end

    xor rcx, rcx                     ; Set rcx to zero

memcpy_loop:

    cmp rcx, rdx                ; if (i == n)
    jge end

    mov r8b, BYTE [rsi]
    mov BYTE [rdi], r8b

    inc rcx
    inc rsi
    inc rdi

    jmp memcpy_loop

end:

    pop r9                      ; Restore r8 and r9 registers
    pop r8

    mov rsp, rbp                    ; Epilogue
    pop rbp

    ret
