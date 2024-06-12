# instructions needed (/../ means done in tokenizer):
# 	push %type (value):                 push stack
# 	pop /%type/:                        pop stack

# 	mov.push (name):                    move value in heap to stack
# 	mov.pop /%type/ (name):             move top of stack to heap
# 	mov %type (name) (value):           move value to heap

# 	free (name):                        free value to heap

# 	add /%type/ /%type/:                pop 2 numbers from the stack and sum push the sum
# 	sub /%type/ /%type/:                pop 2 numbers and push the subtraction

# 	mul /%type /%type/:					pop 2 numbers from the stack and sum push the mul
# 	div /%type/ /%type/:				pop 2 numbers from the stack and sum push the div

# 	shift.left /%type/ amount:			pops the top of the stack and bitshift left "amount" times
# 	shift.right /%type/ amount:			pops the top of the stack and bitshift right "amount" times

# 	print "value":                      print const char*_literal to terminal
# 	print.pop /%type/:                  print the top of the stack and pops it
#   print.peek /%type/:                 print the top of the stack

# 	read %type:                         read number from io input and push it to stack

# 	jump (label):                       jump to label
# 	jump.equals.0 /%type/ (label):      jump to label if top of stack is 0
# 	jump.greater.0 /%type/ (label):     jump to label if top of stack is greater then 0
# 	jump.smaller.0 /%type/ (label):     jump to label if top of stack is smaller then 0

# 	halt:                               stop program

from enum import Enum

class Commands(Enum):
    PUSH = "push"
    POP = "pop"
    MOV_PUSH = "mov.push"
    MOV_POP = "mov.pop"
    MOV = "mov"
    FREE = "free"
    ADD = "add"
    SUB = "sub"
    MUL = "mul"
    DIV = "div"
    SHIFT_LEFT = "shift.left"
    SHIFT_RIGHT = "shift.right"
    PRINT = "print"
    PRINT_POP = "print.pop"
    PRINT_PEEK = "print.peek"
    READ = "read"
    JUMP = "jump"
    JUMP_EQUALS_0 = "jump.equals.0"
    JUMP_GREATER_0 = "jump.greater.0"
    JUMP_SMALLER_0 = "jump.smaller.0"
    HALT = "halt"
    
if __name__ == "__main__":
    print("all commands:")
    for command in Commands:
        print('\t' + command.value)