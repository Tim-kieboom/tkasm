; -- header --
bits 64
default rel

; -- variables --
section .bss

; -- constants --
section .data

; -- Entry Point --
section .text
global main
extern ExitProcess
extern printf
extern scanf

main:
	; -- setup shadow-space --
	PUSH rbp      ; push base pointer to stack
	MOV  rbp, rsp ; set base pointer to stack pointer moving it to the last pointer of the asm stack
	SUB  rsp, 32  ; alloc shadow-space of 32 bytes
	; ----

	; -- PUSH %int8 --
	PUSH 5
	; ----

	; -- HALT --
	JMP EXIT_PROGRAM
	; ----

EXIT_PROGRAM:
	XOR rax, rax
	CALL ExitProcess
