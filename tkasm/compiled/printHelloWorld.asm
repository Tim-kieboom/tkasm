extern _printf
extern _exit

section .data
    msg db 'Hello, World!', 0x0a, 0x00  ; String to print, with newline

section .text
    global _start

_start:
    push msg
    call _printf
    push 0
    call _exit