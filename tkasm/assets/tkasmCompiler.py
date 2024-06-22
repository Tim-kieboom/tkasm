from assets.metaData import *
from assets.Types import *

# BYTE is 8 bits
# WORD is 16 bits
# DWORD is 32 bits
# QWORD is 64 bits

ASM_INIT = \
"""
section .text
    global _start
    
_start:
    setup_StackFrame
"""

def geStrings(string_literals: list[str]) -> str:
    returnString = ""
    for i, string_literal in enumerate(string_literals):
        returnString += f"\tstring_{i} db {string_literal}, 0\n"
        
    return returnString

def replace_BackslashCommand(str: str) -> str:
    return str.replace("\\n", "n").replace("\\t", "t").replace("\\r", "r").replace("\\\"", "\"")

def get_Str_and_StrSize(argument: str, functionVariables: dict[str, Variable], string_map: dict[str, str]) -> tuple[str, int]:
    var = ""
    if argument.__contains__("\""):
        var = argument
    else:
        var = functionVariables[argument].value
    
    size = replace_BackslashCommand(var)
    return string_map[var], len(size)-1

def compile(program: list[str], functions: dict[str, FunctionData], dir_path: str) -> dict[str, int]:
    ip = 0
    
    string_literals: list[str] = []
    string_map: dict[str, str] = {}
    for ip in range(len(program)):
        if program[ip].__contains__("\""):
            string_literal = program[ip].split("\"")[1]
            
            if string_literal == "\\n":
                string_map[program[ip]] = f"newLine"
                continue         
            else:
                string_literal = string_literal.replace("\\n", "\", 10 ,\"")
                
            string_map[program[ip]] = f"string_{len(string_literals)}"
            
            string_literals.append("\""+string_literal+"\"")
    
    file_path: str = dir_path + "\\main.asm"
    out = []
    
    out.extend \
    ([
        "%include \"lib.asm\"\n\n",
        "section .data\n",
        "\tnewLine db 10, 0\n",
        geStrings(string_literals)+"\n",
        "section .bss\n",
        ASM_INIT
    ])
    
    stackSpace_indexes = [len(out)]
    
    functionVariables: dict[str, Variable] = {}
    
    ip = 0
    funcName = "null"
    funcData = FunctionData(VOID, [])
    while ip < len(program):
        token = program[ip]
        type = getType(token)
        
        ip += 1
        
        if token == FUNC:
            funcName = program[ip]
            funcData = functions[funcName]
            
        elif type != UNKNOWN and type != VOID:
            functionVariables[program[ip]] = Variable(program[ip], type, program[ip+1])
            
        elif token == PRINT:
            var, size = get_Str_and_StrSize(program[ip], functionVariables, string_map)    
            out.extend \
            ([
                "\t;----- print -----\n",
                f"\tprint {var}, {size}\n",
                "\t;-----\n\n"
            ])
            
        elif token == PRINTLN:
            var, size = get_Str_and_StrSize(program[ip], functionVariables, string_map)
            out.extend \
            ([
                "\t;----- printLine -----\n",
                f"\tprint {var}, {size}\n",
                f"\tprint newLine, 1\n",
                "\t;-----\n\n"
            ])
            
        elif token == RETURN:
            returnValue = program[ip]
            out.append(f"\n\texit {returnValue}\n")
        
    out.insert(stackSpace_indexes.pop(), f"\tsub rsp, {len(functionVariables) + 4}\n\n")
            
    asmFile = open(file_path, "w")
    asmFile.writelines(out)
    asmFile.close()
    
    return string_map
    