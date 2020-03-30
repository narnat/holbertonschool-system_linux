BITS 64

    global asm_strcasecmp:

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; int asm_strcasecmp(const char *s1, const char *s2);
    ;; @rdi: s1
    ;; @rsi: s2
asm_strcasecmp:

    push rbp
    mov rbp, rsp                ; Prologue

    push r8
    push r9
    push rcx

    xor rcx, rcx                ; Set rcx to 0
    xor eax, eax                ; Set eax to 0
    xor r8d, r8d
    xor r9d, r9d

loop_strcmp:

    mov r8b, BYTE [rdi + rcx]
    mov r9b, BYTE [rsi + rcx]

    ;; jmp convert_case_1

    cmp r8b, 65                 ; Check if char is lowercase
    jl convert_case_2

    cmp r8b, 90
    jg convert_case_2

    add r8b, 32

convert_case_2:

    cmp r9b, 65                 ; Check if char is lowercase
    jl continue

    cmp r9b, 90
    jg continue

    add r9b, 32

continue:

    test r8b, r8b
    je end               ; check if @str1 is null
    test r9b, r9b
    je end

    inc rcx
    cmp r8b, r9b
    je loop_strcmp              ; check if @str1 and @str2 is equal
    jmp end

end:

    mov eax, r8d
    sub eax, r9d

    pop rcx
    pop r9
    pop r8

    mov rsp, rbp
    pop rbp

    ret
