BITS 64

    global asm_strpbrk

    section .text

    ;; rax(rdi, rsi, rdx, r10, r8, r9)
    ;; char *asm_strpbrk(const char *s, const char *accept);
    ;; @rdi: s
    ;; @rsi: accept
asm_strpbrk:

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
    je set_return

    inc rdx
    jmp check_char_loop


check_continue:

    inc rcx
    jmp strspn_loop


set_return:

    mov rax, rdi
    add rax, rcx

end:

    mov rsp, rbp
    pop rbp

    ret
