from assets.metaData import *
from assets.Types import *
from assets.tools.strTools import *
from assets.compile.compileTools import *
from assets.compile.string import *

def compile(program: list[str], functions: dict[str, FunctionData], dir_path: str) -> dict[str, int]:
    ip = 0
    
    input_strings: list[str] = []
    string_literals: list[str] = []
    string_map: dict[str, str] = {}
    get_all_strings(program, string_literals, string_map, input_strings)
    
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
            stackFrame_counter = 0
            
            stackFrame_counter = setupFunction(out, funcData, funcName, stackFrame_counter, stackSpace_indexes)
            
        elif isType(type.value):
            variable = program[ip]
            funcData.addVariable(variable, Variable(variable, type, program[ip+1]))
            stackFrame_counter += type.size
            funcData.variables[variable].stackOffset = stackFrame_counter
            ip+=1
            
            variableInit(out, variable, program[ip], type, funcData, functions, string_map, stackFrame_counter)
            
        elif token == PRINT:
            asm_print(out, program[ip], funcData.variables, string_map)
            out.append("\t;-----\n\n")
            
        elif token == PRINTLN:
            asm_print(out, program[ip], funcData.variables, string_map)
            out.append("\tprint newLine, 1\n")
            out.append("\t;-----\n\n")
        
        elif ALL_OPERATORS.__contains__(token):
            op_type = getType(program[ip])
            value1 = funcData.variables[program[ip+1]].stackOffset
            value2 = funcData.variables[program[ip+2]].stackOffset
            doOperator(out, token, op_type, value1, value2)
            ip+= 2
         
        elif token == RETURN:
            value = program[ip]
            returnFunc(out, funcName, value, funcData)
            
        elif token == END_FUNC:
            setup_stackPointer_to_StackFrame(funcName, out, funcData.variables, stackSpace_indexes)
       
    asmFile = open(file_path, "w")
    asmFile.writelines(out)
    asmFile.close()
    
    return string_map
    