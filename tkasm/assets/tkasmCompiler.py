from assets.command import Commands

# [] in asm means value of pointer
# qword 8 bytes

def geStrings(string_literals: list[str]) -> str:
    returnString = ""
    for i, string_literal in enumerate(string_literals):
        returnString += f"string_literal_{i} db {string_literal}, 0\n"
        
    return returnString

def compile(program: list[str], dir_path: str) -> None:
    ip = 0
    string_literals: list[str] = []
    for ip in range(len(program)):
        if program[ip] == Commands.PRINT.value:
            string_literal = program[ip+1]
            program[ip+1] = len(string_literals)
            string_literals.append(string_literal)
    
    file_path: str = dir_path + "\\main.asm"
    out = open(file_path, "w")
    
    out.write \
    (
        "; -- header --\n"                                                                                          
        "bits 64\n"                                                                                                 
        "default rel\n"                                                                                             
        "\n"                                                                                                        
        "; -- variables --\n"                                                                                       
        "section .bss\n"                                                                                            
        "\n"                                                                                                        
        "; -- constants --\n"                                                                                       
        "section .data\n" +
        geStrings(string_literals) +                                                                                
        "\n"                                                                                                        
        "; -- Entry Point --\n"                                                                                     
        "section .text\n"                                                                                           
        "global main\n"                                                                                             
        "extern ExitProcess\n"                                                                                      
        "extern printf\n"                                                                                           
        "extern scanf\n"                                                                                            
        "\n"                                                                                                        
        "main:\n"                                                                                                   
            "\t; -- setup shadow-space --\n"                                                                        
            "\tPUSH rbp      ; push base pointer to stack\n"                                                        
            "\tMOV  rbp, rsp ; set base pointer to stack pointer moving it to the last pointer of the asm stack\n"  
            "\tSUB  rsp, 32  ; alloc shadow-space of 32 bytes\n"                                                    
            "\t; ----\n"                                                                    
        "\n"
    )
    
    ip = 0
    while ip < len(program):
        command = program[ip]
        ip += 1
        
        if command.endswith(":"):
            out.write(f"; -- label --\n")
            out.write(f"{command}:\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.PUSH.value:
            type = program[ip]
            number = program[ip+1]
            ip += 2
            out.write(f"\t; -- PUSH {type} --\n")
            out.write(f"\tPUSH {number}\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.POP.value:
            type = program[ip]
            ip += 1
            out.write(f"\t; -- POP {type} --\n")
            out.write(f"\tPOP\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.ADD.value:
            type1 = program[ip]
            type2 = program[ip+1]
            ip += 2
            out.write(f"\t; -- ADD {type1}->{type2} --\n")
            out.write(f"\tPOP rax\n")
            out.write(f"\tADD qword [rsp], rax\n")
            out.write(f"\t; ----\n\n")
    
        elif command == Commands.SUB.value:
            type1 = program[ip]
            type2 = program[ip+1]
            ip += 2
            out.write(f"\t; -- SUB {type1}->{type2} --\n")
            out.write(f"\tPOP rax\n")
            out.write(f"\tSUB qword [rsp], rax\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.PRINT.value:
            string_index = program[ip]
            ip += 1
            out.write(f"\t; -- PRINT--\n")
            out.write(f"\tLEA rcx, string_literal_{string_index}\n")
            out.write(f"\tXOR eax, eax\n")
            out.write(f"\tCALL printf\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.PRINT_POP.value:
            type = program[ip]
            ip += 1
            out.write(f"\t; -- PRINT_POP {type} --\n")
            out.write(f"\tPOP rdi\n")
            out.write(f"\tCALL printf\n")
            out.write(f"\t; ----\n\n")
          
        elif command == Commands.PRINT_PEEK.value:
            type = program[ip]
            ip += 1
            out.write(f"\t; -- PRINT_POP {type} (not sure if works) --\n")
            out.write(f"\tPOP rdi\n")
            out.write(f"\tCALL printf\n")
            out.write(f"\tPUSH rdi\n")
            out.write(f"\t; ----\n\n")
    
        elif command == Commands.READ.value:
            type = program[ip]
            ip += 1
            out.write(f"\t; -- READ {type} --\n")
            out.write(f"\tPUSH rdi\n")
            out.write(f"\tCALL scanf\n")
            out.write(f"\tPOP rdi\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.JUMP_EQUALS_0.value:
            type = program[ip]
            label = program[ip+1]
            ip += 2
            out.write(f"\t; -- JUMP_EQUALS_0 {type} --\n")
            out.write(f"\tCMP qword [rsp], 0\n")
            out.write(f"\tJE {label}\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.JUMP_GREATER_0.value:
            type = program[ip]
            label = program[ip+1]
            ip += 2
            out.write(f"\t; -- JUMP_GREATER_0 {type} --\n")
            out.write(f"\tCMP qword [rsp], 0\n")
            out.write(f"\tJG {label}\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.JUMP_SMALLER_0.value:
            type = program[ip]
            label = program[ip+1]
            ip += 2
            out.write(f"\t; -- JUMP_SMALLER_0 {type} --\n")
            out.write(f"\tCMP qword 0, [rsp]\n")
            out.write(f"\tJG {label}\n")
            out.write(f"\t; ----\n\n")
            
        elif command == Commands.HALT.value:
            out.write(f"\t; -- HALT --\n")
            out.write(f"\tJMP EXIT_PROGRAM\n")
            out.write(f"\t; ----\n\n")
            
    out.write(f"EXIT_PROGRAM:\n")
    out.write(f"\tXOR rax, rax\n")
    out.write(f"\tCALL ExitProcess\n")
    
    out.close()
    