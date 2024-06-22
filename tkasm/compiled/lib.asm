section .bss
	printVal_digitSpace resb 100
	printVal_digitSpacePos resb 8

%macro setup_StackFrame 0
    push rbp
    mov rbp, rsp 
%endmacro

%macro exit 1
    mov rax, 60
    mov rdi, %1
    syscall
%endmacro 

%macro print 2
    mov rax, SYS_WRITE      
    mov rdi, STDOUT     
    mov rsi, %1   
    mov rdx, %2
    syscall
%endmacro 

%macro printVal 0
    mov rcx, printVal_digitSpace
    mov [printVal_digitSpacePos], rcx

    %%divide_loop:
        mov rdx, 0
        mov rbx, 10
        div rbx
        push rax
        add rdx, 48

        mov rcx, [printVal_digitSpacePos]
        mov [rcx], dl
        inc rcx
        mov [printVal_digitSpacePos], rcx
        
        pop rax
        cmp rax, 0
        jne %%divide_loop

    %%printValueLoop:
        mov rcx, [printVal_digitSpacePos]

        mov rax, SYS_WRITE
        mov rdi, STDOUT
        mov rsi, rcx
        mov rdx, 1
        syscall

        mov rcx, [printVal_digitSpacePos]
        dec rcx
        mov [printVal_digitSpacePos], rcx

        cmp rcx, printVal_digitSpace
        jge %%printValueLoop
%endmacro

STDIN_FILENO	equ	0
STDOUT_FILENO	equ	1
STDERR_FILENO	equ	2
STDIN	        equ	0
STDOUT	        equ	1
STDERR	        equ	2

SYS_READ	    equ	0
SYS_WRITE	    equ	1
SYS_OPEN	    equ	2
SYS_CLOSE	    equ	3