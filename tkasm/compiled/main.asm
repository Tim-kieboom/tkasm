%include "lib.asm"

section .data
	newLine db 10, 0
	string_0 db "hello world", 0

section .bss
	input_0 resb 20


section .text
    global _start
    
add:
	setup_StackFrame
	sub rsp, 12 ; sizeof_vars(12Bytes)

	;---- set args ----
	mov [rbp-4], eax ;a
	mov [rbp-8], ebx ;b
	;-----

	;----- operator * -----
	mov eax, [rbp-4]
	mov edx, [rbp-8]
	mul edx
	;-----

	;----- setValue ab -----
	mov DWORD [rbp-12], eax
	;-----

	mov eax, [rbp-12]
	cleanup_StackFrame
	ret


_start:
    xor rbp, rbp
	setup_StackFrame
	sub rsp, 12 ; sizeof_vars(8Bytes) + 4Bytes for shadowSpace

	;----- input -----
	input input_0, 20
	;-----

	;----- setValue bar -----
	mov DWORD [rbp-4], 1
	;-----

	;----- call add -----
	mov eax, 1 ;a
	mov ebx, 2 ;b
	call add
	mov [rbp-8], eax
	;-----

	;----- print -----
	printVal_32 [rbp-8]
	print newLine, 1
	;-----

	;----- print -----
	printVal_32 [rbp-4]
	print newLine, 1
	;-----

	cleanup_StackFrame
	exit 0

