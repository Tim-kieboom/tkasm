section .data
    minus db "-", 0

section .bss
	printVal_digitSpace resb 100
	printVal_digitSpacePos resb 8
    buffer_20 resb 20

%macro setup_StackFrame 0
    push rbp
    mov rbp, rsp 
%endmacro

%macro cleanup_StackFrame 0
    mov rsp, rbp
    pop rbp
%endmacro

%macro exit 1
    mov rax, 60
    mov rdi, %1
    syscall
%endmacro 

%macro input 2
    mov rax, SYS_READ
    mov rdi, STDIN
    mov rsi, %1   
    mov rdx, %2
    syscall
%endmacro 

%macro print 2
    mov rax, SYS_WRITE      
    mov rdi, STDOUT     
    mov rsi, %1   
    mov rdx, %2
    syscall
%endmacro 

%macro printVal_64 1
mov rax, %1
prv_printVal
%endmacro

%macro printVal_32 1
movsx rax, DWORD %1
prv_printVal
%endmacro

%macro printVal_16 1
movsx rax, WORD %1
prv_printVal
%endmacro

%macro printVal_8 1
movsx rax, BYTE %1
prv_printVal
%endmacro

%macro prv_printVal 0
test rax, rax            ; Test if the number is negative
jns %%int_to_string      ; If the number is non-negative, jump to .positive

;---- Handle negative number ----
push rax                 ; Save the original number on the stack
print minus, 1
pop rax                  ; Restore the original number
neg rax                  ; Negate the number to make it positive
;----

%%int_to_string:
    mov rdi, buffer_20    
    mov rbx, 10         ; Divisor for converting to decimal
    xor rcx, rcx        ; Clear RCX (digit_counter)

%%divide_loop:
    xor rdx, rdx        ; Clear RDX for division
    div rbx             ; Divide RAX by 10
    push rdx            ; Push remainder (digit) onto stack
    inc rcx             ; Increment digit_counter
    test rax, rax       ; Check if quotient is zero
    jnz %%divide_loop   ; If not, continue dividing

%%build_string:
    mov rdx, rcx        ; move digit_counter to rdx for later to print

    pop rax            
    add al, '0'         ; Convert to ASCII
    mov [rdi], al       ; Store in buffer
    inc rdi             ; Move to next buffer position
    loop %%build_string ; Loop until all digits are processed (loop uses rcx)

    print buffer_20, rdx
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