BITS 64

    global asm_strcasecmp:

    section .text

asm_strcasecmp:

    push rbp
    mov rbp, rsp                ; Prologue

    xor rcx, rcx                ; Set rcx to 0
    xor eax, eax                ; Set eax to 0
    xor r8, r8
    xor r9, r9

loop_strcmp:

    mov r8b, BYTE [rdi + rcx]
    mov r9b, BYTE [rsi + rcx]

    inc rcx

    test r8b, r8b
    je end               ; check if @str1 is null

    jmp convert_case_1

continue:

    cmp r8b, r9b
    je loop_strcmp              ; check if @str1 and @str2 is equal
    jmp end

convert_case_1:

    cmp r8b, 97                 ; Check if char is lowercase
    jl convert_case_2

    cmp r8b, 122
    jg convert_case_2

    sub r8b, 32

convert_case_2:

    cmp r9b, 97                 ; Check if char is lowercase
    jl continue

    cmp r9b, 122
    jg continue

    sub r9b, 32
    jmp continue

end:

    mov eax, r8d
    sub eax, r9d

    mov rsp, rbp
    pop rbp

    ret
