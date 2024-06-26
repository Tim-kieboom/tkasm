from assets.Types import Type, getType

FUNC = r"%func"
END_FUNC = r"%end_func"
RETURN = r"%return"
PRINT = r"%print"
PRINTLN = r"%println"
CALL = r"%call"

INPUT = "input"

ALL_COMMAND = [FUNC, END_FUNC, RETURN, PRINT, PRINTLN, INPUT]

ADD = "+"
SUB = "-"
MUL = "*"
DIV = "/"

NO_OPERATION = "no_op"
OPERATION_VALUE = "op_value"

ALL_OPERATORS = [ADD, SUB, MUL, DIV]

REG_AX  = "ax"
REG_BX  = "bx"
REG_DI  = "di"
REG_SI  = "si"
REG_DX  = "dx"
REG_CX  = "cx"

REG_RBP = "rbp"
REG_RSP = "rsp"
REG_R8  = "r8"
REG_R9  = "r9"
REG_R10 = "r10"


REGISTER_64ONLY = [REG_RBP, REG_RSP, REG_R8, REG_R9, REG_R10]

ARG_REGISTERS = [REG_AX, REG_BX, REG_DI, REG_SI, REG_DX, REG_CX, REG_R8, REG_R9, REG_R10]

ALL_REGISTERS = [REG_AX, REG_BX, REG_DI, REG_SI, REG_DX, REG_CX, REG_RBP, REG_RSP, REG_R8, REG_R9, REG_R10]

def isCommand(str: str) -> bool:
    return ALL_COMMAND.__contains__(str) 

class Variable:
    def __init__(self, name: str, type: Type, value: str):
        self.name: str = name
        self.type: Type = type
        self.value: str = value
        self.stackOffset: int = -1

class Argument:
    def __init__(self, type: Type, name: str):
        self.name: str = name
        self.type: Type = getType(type)
        
    def print(self):
        print(f"\t\t{self.type.value} {self.name}")
        
class FunctionData:
    def __init__(self, type: Type, args: list[Argument]):
        self.args: list[Argument] = args
        self.type: Type = type
        self.variables: dict[str, Variable] = {}
        
    def addArg(self, name: str, type: Type):
        self.args.append(Argument(name, type))
        
    def addVariable(self, name: str, var: Variable):
        self.variables[name] = var
        
    def print(self):
        print(f"\ttype: {self.type.value}")
        print("\targs: ")
        for arg in self.args:
            arg.print()
        
def args_str(argLine: str) -> list[list[str]]:
    argsStr = []
    
    argLine = argLine.split("(")[1]
    argLine = argLine.replace(")", "").replace("\n", "")
    
    rawArgs: list[str] = argLine.split(",")
    for arg in rawArgs:
        if arg[0] == ' ': 
            arg = arg[1:]
            
        var = arg.split(" ")
        argsStr.append([var[0], var[1]])
    return argsStr

def get_functionData(line: str, type: Type) -> FunctionData:
    argLine = line.split("(")[1]
    argLine = argLine.replace(")", "").replace("\n", "")
    
    args: list[Argument] = []
    rawArgs: list[str] = argLine.split(",")
    for arg in rawArgs:
        if arg[0] == ' ': 
            arg = arg[1:]
            
        var = arg.split(" ")
        args.append(Argument(var[0], var[1]))
       
    return FunctionData(type, args)

def get_operator(line: str) -> str:
    
    if line.__contains__(ADD):
        return ADD
    if line.__contains__(SUB):
        return SUB
    if line.__contains__(MUL):
        return MUL
    if line.__contains__(DIV):
        return DIV
    
    return NO_OPERATION

def get_type_register(type: Type, register: str) -> str:
    size = type.size
    
    if REGISTER_64ONLY.__contains__(register):
        print(f"!!<error> get_type_register() {register} register is 64bit only !!")
        exit(1)
    
    if size == 0:
        return ""
    
    elif size == 1:
        register[-1] = 'l'
        return register
    
    elif size == 2:
        return register
    
    elif size == 4:
        return "e"+register
    
    elif size == 8:
        return "r"+register