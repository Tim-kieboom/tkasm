from assets.metaData import *
from assets.Types import *
from assets.tools.strTools import *

# BYTE is 8 bits
# WORD is 16 bits
# DWORD is 32 bits
# QWORD is 64 bits

ASM_INIT = \
"""
section .text
    global _start
"""

ASM_MAIN = "_start:\n"

def getStrings(string_literals: list[str]) -> str:
    returnString = ""
    for i, string_literal in enumerate(string_literals):
        returnString += f"\tstring_{i} db {string_literal}, 0\n"
        
    return returnString

def getInputStrings(input_strings: list[str]) -> str:
    returnString = ""
    for i, input_string in enumerate(input_strings):
        returnString += f"\tinput_{i} resb {input_string}\n"
        
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

def containsCall(token: str) -> bool:
    return token.__contains__("(")

def asm_print(out: list[str], varName: str, functionVariables: dict[str, Variable], string_map: dict[str, str]):
    variable: Variable = functionVariables[varName]
            
    if variable.type == STRING:
        var, size = get_Str_and_StrSize(varName, functionVariables, string_map)    
        out.extend \
        ([
            "\t;----- print -----\n",
            f"\tprint {var}, {size}\n",
        ])
    else:
        out.extend \
        ([
            "\t;----- print -----\n",
            f"\tprintVal [rbp-{variable.stackOffset}]\n",
        ])

def setup_stackPointer_to_StackFrame(funcName: str, out: list[str], functionVariables: dict[str, Variable], stackSpace_indexes: list[int]):
    varSizes = 0  
    for key in functionVariables:
        varSizes += functionVariables[key].type.size
    
    if funcName == "main":
        out.insert(stackSpace_indexes.pop(), f"\tsub rsp, {varSizes + 4} ; sizeof_vars({varSizes}Bytes) + 4Bytes for shadowSpace\n\n")
    else:
        out.insert(stackSpace_indexes.pop(), f"\tsub rsp, {varSizes} ; sizeof_vars({varSizes}Bytes)\n\n")

def compile(program: list[str], functions: dict[str, FunctionData], dir_path: str) -> dict[str, int]:
    ip = 0
    
    input_strings: list[str] = []
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
            
        elif program[ip] == INPUT:
            string_size = program[ip+1]
            
            string_map[program[ip-1]] = f"input_{len(input_strings)}"
            input_strings.append(string_size)
    
    file_path: str = dir_path + "\\main.asm"
    out = []
    
    out.extend \
    ([
        "%include \"lib.asm\"\n\n",
        "section .data\n",
        "\tnewLine db 10, 0\n",
        getStrings(string_literals)+"\n",
        "section .bss\n",
        getInputStrings(input_strings)+"\n",
        ASM_INIT
    ])
    
    stackSpace_indexes = []
    
    functionVariables: dict[str, Variable] = {}
    
    ip = 0
    funcName = "null"
    stackFrame_counter = 0
    funcData = FunctionData(VOID, [])
    while ip < len(program):
        token = program[ip]
        type = getType(token)
        
        ip += 1
        
        if token == FUNC:
            funcName = program[ip]
            funcData = functions[funcName]
            
            funcLabel = ASM_MAIN if funcName == "main" else funcName+":\n"
            
            out.append(funcLabel)
            out.append("\tsetup_StackFrame\n")
            stackSpace_indexes.append(len(out))
            
        elif type != UNKNOWN and type != VOID:
            variable = program[ip]
            functionVariables[variable] = Variable(variable, type, program[ip+1])
            ip+=1
            
            if not containsCall(program[ip]) and type != STRING:
                stackFrame_counter += type.size
                functionVariables[variable].stackOffset = stackFrame_counter
                out.extend \
                ([
                    "\t;----- setValue -----\n",
                    f"\tmov {type_to_asmType(type)} [rbp-{stackFrame_counter}], {program[ip]}\n",
                    "\t;-----\n\n"
                ])
            
        elif token == PRINT:
            asm_print(out, program[ip], functionVariables, string_map)
            out.append("\t;-----\n\n")
            
        elif token == PRINTLN:
            asm_print(out, program[ip], functionVariables, string_map)
            out.extend \
            ([
                "\tprint newLine, 1\n",
                "\t;-----\n\n"
            ])
         
        elif token == INPUT:
            if isType(program[ip-3]):
                functionVariables[program[ip-2]] = Variable(program[ip-2], type, program[ip-2])
            
            var = string_map[program[ip-2]]
            size = program[ip]
            out.extend \
            ([
                "\t;----- input -----\n",
                f"\tinput {var}, {size}\n",
                "\t;-----\n\n"
            ])
            
        elif token == RETURN:
            value = program[ip]
            out.append("\tcleanup_StackFrame")
            
            if funcName == "main":
                out.append(f"\n\texit {value}\n\n")
            else:
                returnValue = "" if value == "" else f"[rbp-{functionVariables[value].stackOffset}]"
                out.append(f"\n\tret {returnValue}\n\n")
            
        elif token == END_FUNC:
            setup_stackPointer_to_StackFrame(funcName, out, functionVariables, stackSpace_indexes)
     
        
           
            
    asmFile = open(file_path, "w")
    asmFile.writelines(out)
    asmFile.close()
    
    return string_map
    