from assets.tools.strTools import *
from assets.metaData import *
from assets.Types import *

def check_If_Func_Has_Body(parts: list[str], file: list[str], lineNumber: int) -> None:
    if parts.__len__() > 2 and parts[2][0] == '{':
        return
            
    elif file.__len__() > lineNumber+1 and file[lineNumber+1][0] == '{':
        return
    
    else:
        print(f"!!<error> line: {lineNumber+1} func has no body!!")
        exit(1)

        
def begin_func(program: list[str], parts: list[str], functions: dict[FunctionData]) -> None:
    program.append(FUNC)
    program.append(parts[1].split("(")[0])
        
    name = parts[1].split("(")[0]
    functions[name] = get_functionData(parts[1], getType(parts[0]))     
    
   
def is_variable(parts: list[str]) -> bool:
    return parts[-2].__contains__("=") 
        

def tokenize(file: list[str]):
    program: list[str] = []
    
    functions: dict[FunctionData] = {}
    inFunc = [False] # list to make reference
    
    for lineNumber, line in enumerate(file):
        tokenizeLine(line, lineNumber, program, functions, inFunc, file)

         
    return program, functions
            
def tokenizeLine(line: str, lineNumber: int, program: list[str], functions: dict[FunctionData], inFunc: list[bool], file: list[str]):
    line = line.replace(";", " ")
    parts = split_without_brackets(line)
    parts = filter_out_comments(parts)
    
    if(parts.__len__() == 0):
        return
        
            
    type: Type = getType(parts[0])
    if type != UNKNOWN:
        
        if parts.__len__() < 2:
            print(f"!!<error> line: {lineNumber+1} only has a type!!")
            exit(1)
        
        if is_variable(parts):
            parts.pop(2)
            program.extend(parts)
        
        if containsBrackets(parts[1]):
            begin_func(program, parts, functions)
            check_If_Func_Has_Body(parts, file, lineNumber)
            inFunc[0] = True
            return
    
    elif parts[0][:5] == "print":
        
        if parts[0][5:7] == "ln": 
            program.append(PRINTLN)
        else:
            program.append(PRINT)
        
        variable = parts[0].replace(")", "") \
                           .replace(";", "") \
                           .split("(")[1]
        program.append(variable)
    
    elif parts[0] == "return":
        program.append(RETURN)
        program.append(parts[1])
        
    elif parts.__contains__("}"):
        if not inFunc:
            print(f"!!<error> line: {lineNumber+1} "+"has \'}\' but no \'{\' !!")
            exit(1)
        
        inFunc[0] = False
        program.append(END_FUNC)
        return
    
    elif parts[0] == "{":
        return
    
    else:
        program.append("!-! not implemented !-!")
    
    