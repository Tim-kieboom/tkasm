%include "lib.asm"

section .data
	newLine db 10, 0
	string_0 db "hello world", 0

section .bss


section .text
    global _start
add:
	setup_StackFrame
	sub rsp, 4 ; sizeof_vars(4Bytes)

	;----- setValue -----
	mov DWORD [rbp-4], a+b
	;-----

	cleanup_StackFrame
	ret [rbp-4]

_start:
	setup_StackFrame
	sub rsp, 12 ; sizeof_vars(8Bytes) + 4Bytes for shadowSpace

	;----- setValue -----
	mov DWORD [rbp-8], 1
	;-----

	;----- print -----
	printVal [rbp-8]
	print newLine, 1
	;-----

	cleanup_StackFrame
	exit 0

