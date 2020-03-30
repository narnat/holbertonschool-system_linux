BITS 64

    global asm_strncasecmp:

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; int asm_strncasecmp(const char *s1, const char *s2, size_t n);
    ;; @rdi: s1
    ;; @rsi: s2
    ;; @rdx: n
asm_strncasecmp:

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

    inc rcx

    test r8b, r8b
    je end               ; check if @str1 is null
    test r9b, r9b
    je end

    cmp r8b, 65                 ; Check if char is lowercase
    jl convert_case

    cmp r8b, 90
    jg convert_case

    add r8b, 32

convert_case:

    cmp r9b, 65                 ; Check if char is lowercase
    jl continue

    cmp r9b, 90
    jg continue

    add r9b, 32

continue:

    cmp rcx, rdx
    je end
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
