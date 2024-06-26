from assets.metaData import *
from assets.Types import *
from assets.tools.strTools import *
from assets.compile.string import *

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
        typeSize = variable.type.size*8
        out.extend \
        ([
            "\t;----- print -----\n",
            f"\tprintVal_{typeSize} [rbp-{variable.stackOffset}]\n",
        ])



def doOperator(out: list[str], token: str, type: Type, value1, value2):
    register1 = get_type_register(type, REG_AX)
    register2 = get_type_register(type, REG_DX)
    
    if token == ADD:
        asm_op = f"add {register1},"
    elif token == SUB:
        asm_op = f"sub {register1},"
    elif token == MUL:
        asm_op = "mul"
    elif token == DIV:
        asm_op = "div"
    else:
        asm_op = f"mov {register1},"
    
    out.extend \
    ([
        f"\t;----- operator {token} -----\n",
        f"\tmov {register1}, [rbp-{value1}]\n",
        f"\tmov {register2}, [rbp-{value2}]\n",
        f"\t{asm_op} {register2}\n",
        "\t;-----\n\n"
    ])
    
def get_args_in_registers(callFuncName: str, functions: dict[str, FunctionData], variables: dict[str, Variable], callArgs: list[str]) -> list[str]:
    args: list[Argument] = functions[callFuncName].args
    
    if len(args) < len(callArgs):
        raise Exception("too few arguments")
    
    if len(args) > len(ARG_REGISTERS):
        raise Exception("too many arguments")
    
    strings = [] 
    for i, arg in enumerate(args):
        var_Or_Value = callArgs[i]
        if variables.get(var_Or_Value, False):
            strings.append(f"\fmov {get_type_register(arg.type, ARG_REGISTERS[i])}, [rbp-{variables[var_Or_Value].stackOffset}] ;{arg.name}\n")
        else:
            value = var_Or_Value
            
        strings.append(f"\tmov {get_type_register(arg.type, ARG_REGISTERS[i])}, {value} ;{arg.name}\n")
        
    return strings

def setup_stackPointer_to_StackFrame(funcName: str, out: list[str], functionVariables: dict[str, Variable], stackSpace_indexes: list[int]):
    varSizes = 0  
    for key in functionVariables:
        varSizes += functionVariables[key].type.size
    
    if funcName == "main":
        out.insert(stackSpace_indexes.pop(), f"\tsub rsp, {varSizes + 4} ; sizeof_vars({varSizes}Bytes) + 4Bytes for shadowSpace\n\n")
    else:
        out.insert(stackSpace_indexes.pop(), f"\tsub rsp, {varSizes} ; sizeof_vars({varSizes}Bytes)\n\n")
 
def getInput(out: list[str], type: Type, varName: str, value: str, funcData: FunctionData, string_map: dict[str, str]):
    if isType(type.value):
        funcData.addVariable(varName, Variable(varName, type, varName))
    
    var = string_map[varName]
    size = value.split("(")[-1].replace(")", "")
    out.extend \
    ([
        "\t;----- input -----\n",
        f"\tinput {var}, {size}\n",
        "\t;-----\n\n"
    ])
def variableInit(out: list[str], varName: str, value: str, type: Type, funcData: FunctionData, functions: dict[str, FunctionData], string_map: dict[str, str], stackFrame_counter: int):        
    command = value.split("(")[0]
        
    if isCommand(command):
        
        if command == INPUT:
            getInput(out, type, varName, value, funcData, string_map)
            
        else:
            print("func isCommand not yet implemented")
        
    elif type == STRING:
        return 
    
    elif not containsCall(value):
        setValue(varName, value, funcData, out, type, stackFrame_counter)
        
    else:
        callFunction(out, value, type, functions, funcData, stackFrame_counter)
        
def callFunction(out: list[str], callLine: str, type: Type, functions: dict[str, FunctionData], funcData: FunctionData, stackFrame_counter: int):
    callParts = callLine.split("(")
    callName = callParts[0]
    callArgs = callParts[1].replace(")", "").split(",")
    
    out.append(f"\t;----- call {callName} -----\n")
    out.extend(get_args_in_registers(callName, functions, funcData.variables, callArgs))
    out.extend \
    ([
        f"\tcall {callName}\n",
        f"\tmov [rbp-{stackFrame_counter}], {get_type_register(type, REG_AX)}\n",
        "\t;-----\n\n"
    ]) 
    
def setValue(varName: str, value: str, funcData: FunctionData, out: list[str], type: Type, stackFrame_counter: int):
    if funcData.variables.get(value, False):
        out.extend \
        ([
            f"\t;----- setValue {varName} -----\n",
            f"\tmov {get_type_register(type, REG_BX)}, [rbp-{stackFrame_counter}]\n",
            f"\tmov {type_to_asmType(type)} [rbp-{stackFrame_counter}], {get_type_register(type, REG_BX)}\n",
            "\t;-----\n\n"
        ])
        return
    
    if value == OPERATION_VALUE:
        value = get_type_register(type, REG_AX)
        
    else:
        value = value
    
    out.extend \
    ([
        f"\t;----- setValue {varName} -----\n",
        f"\tmov {type_to_asmType(type)} [rbp-{stackFrame_counter}], {value}\n",
        "\t;-----\n\n"
    ])

def setupFunction(out: list[str], funcData: FunctionData, funcName: str, stackFrame_counter: int, stackSpace_indexes: list[int]) -> int:
    if funcName == "main":
        out.append(ASM_MAIN)
        out.append("\tsetup_StackFrame\n")
        stackSpace_indexes.append(len(out))
            
    else:
        out.append(funcName+":\n")
        out.append("\tsetup_StackFrame\n")
        stackSpace_indexes.append(len(out))
        
        out.append("\t;---- set args ----\n")
        for i, arg in enumerate(funcData.args):
            funcData.addVariable(arg.name, Variable(arg.name, arg.type, ""))
            stackFrame_counter += arg.type.size
            funcData.variables[arg.name].stackOffset = stackFrame_counter
            
            out.append(f"\tmov [rbp-{stackFrame_counter}], {get_type_register(arg.type, ARG_REGISTERS[i])} ;{arg.name}\n")
        out.append("\t;-----\n\n")
        
    return stackFrame_counter

def returnFunc(out: list[str], funcName: str, value: str, funcData: FunctionData):
    if funcName == "main":
        out.append("\tcleanup_StackFrame")
        out.append(f"\n\texit {value}\n\n")
    else:
        if value != "":
            type = funcData.variables[value].type if value != "" else VOID
            stackAddress = f"[rbp-{funcData.variables[value].stackOffset}]"
            out.append(f"\tmov {get_type_register(type, REG_AX)}, {stackAddress}\n")
        
        out.append("\tcleanup_StackFrame")
        out.append(f"\n\tret\n\n")