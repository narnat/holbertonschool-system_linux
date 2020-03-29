BITS 64

    global asm_strcspn

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; size_t asm_strcspn(const char *s, const char *reject);
    ;; @rdi: s
    ;; @rsi: reject
asm_strcspn:

    push rbp
    mov rbp, rsp

    xor rax, rax
    xor rcx, rcx

    test rdi, rdi               ; Test for NULL, actual strspn doesn't do it
    jz end
    test rsi, rsi
    jz end

strspn_loop:

    mov r8b, BYTE [rdi + rcx]
    test r8b, r8b
    jz end

    xor rdx, rdx                ; Start of inner loop

check_char_loop:

    mov r9b, BYTE [rsi + rdx]
    test r9b, r9b
    jz check_continue

    cmp r8b, r9b
    je end

    inc rdx
    jmp check_char_loop


check_continue:

    inc rcx
    jmp strspn_loop

end:

    mov rax, rcx

    mov rsp, rbp
    pop rbp

    ret
