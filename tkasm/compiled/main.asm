%include "lib.asm"

section .data
	newLine db 10, 0
	string_0 db "hello world", 0
	string_1 db "foo", 0
	string_2 db "test", 0

section .bss

section .text
    global _start
    
_start:
    setup_StackFrame
	sub rsp, 6

	;----- print -----
	print string_2, 5
	;-----

	;----- print -----
	print newLine, 2
	;-----

	;----- printLine -----
	print string_0, 12
	print newLine, 1
	;-----


	exit 0
