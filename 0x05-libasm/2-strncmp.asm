BITS 64

    global asm_strncmp:

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; int asm_strncmp(const char *s1, const char *s2, size_t n);
    ;; @rdi: s1
    ;; @rsi: s2
    ;; @rdx: n
asm_strncmp:

    push rbp
    mov rbp, rsp                ; Prologue

    xor rcx, rcx                ; Set rcx to 0
    xor eax, eax                ; Set eax to 0

loop_strcmp:

    mov r8b, BYTE [rdi + rcx]
    mov r9b, BYTE [rsi + rcx]
    inc rcx

    cmp rdx, rcx
    je end_strcmp
    test r8b, r8b
    je check_null               ; check if @str1 is null
    ;; test r9b, r9b
    ;; je set_one               ; check if @str2 is null
    cmp r8b, r9b
    je loop_strcmp              ; check if @str1 and @str2 is equal
    jmp compare

check_null:

    test r9b, r9b
    je end_strcmp
    jmp set_negative

compare:

    cmp r8b, r9b
    jl set_negative
    jmp set_one

set_one:

    mov eax, 1
    jmp end_strcmp

set_negative:

    mov eax, -1                 ; Set -1 one
    jmp end_strcmp

end_strcmp:

    mov rsp, rbp
    pop rbp

    ret
