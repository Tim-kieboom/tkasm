from assets.metaData import *

ASM_INIT = \
"""
section .text
    global _start
    
"""

ASM_MAIN = \
"""
_start:
    xor rbp, rbp
"""

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

def get_all_strings(program: list[str], string_literals: list[str], string_map: dict[str, str], input_strings: list[str]):
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
            string_size = program[ip+1].split("(")[-1].replace(")", "")
            
            string_map[program[ip]] = f"input_{len(input_strings)}"
            input_strings.append(string_size)
            ip += 1