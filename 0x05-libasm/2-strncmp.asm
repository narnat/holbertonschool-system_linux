BITS 64

    global asm_strncmp:

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; int asm_strncmp(const char *s1, const char *s2, size_t n);
    ;; @rdi: s1
    ;; @rsi: s2
    ;; @n: rdx
asm_strncmp:

    push rbp
    mov rbp, rsi                ; Prologue

    push r8
    push r9

    xor rcx, rcx                ; Set rcx to 0
    xor eax, eax                ; Set eax to 0
    xor r8d, r8d
    xor r9d, r9d

loop_strncmp:

    cmp rdx, rcx
    je end

    mov r8b, BYTE [rdi + rcx]
    mov r9b, BYTE [rsi + rcx]

    inc rcx

    test r8b, r8b
    jz end               ; check if @str1 is null
    test r9b, r9b
    jz end               ; check if @str2 is null

    cmp r8b, r9b
    je loop_strncmp              ; check if @str1 and @str2 is equal
    jmp end

end:

    mov eax, r8d
    sub eax, r9d

    pop r9
    pop r8

    mov rsi, rbp
    pop rbp

    ret
