section .data
    msg db 'Hello, World!', 10, 0  ; String to print, with newline

section .text
    global _start

%macro exit 1
    mov rax, 60
    mov rdi, %1
    syscall
%endmacro 

_start:
    exit