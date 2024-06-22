from assets.Types import Type, getType

FUNC = r"%func"
END_FUNC = r"%end_func"

RETURN = r"%return"
PRINT = r"%print"
PRINTLN = r"%println"

ALL_COMMAND = [FUNC, END_FUNC, RETURN, PRINT, PRINTLN]

def isCommand(str: str) -> bool:
    return ALL_COMMAND.__contains__(str) 

class Variable:
    def __init__(self, name: str, type: Type, value: str):
        self.name: str = name
        self.type: Type = type
        self.value:str = value

class Argument:
    def __init__(self, type: str, name: str):
        self.name: str = name
        self.type: Type = getType(type)
        
class FunctionData:
    def __init__(self, type: Type, args: list[Argument]):
        self.args: list[Argument] = args
        self.type: str = type
        
    def addArg(self, name: str, type: Type):
        self.args.append(Argument(name, type))
        
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