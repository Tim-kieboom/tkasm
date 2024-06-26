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
    for part in parts:
        if part.__contains__("="):
            return True
        

def tokenize(file: list[str]):
    program: list[str] = []
    
    functions: dict[FunctionData] = {}
    inFunc = [False] # list to make reference
    
    for lineNumber, line in enumerate(file):
        tokenizeLine(line, lineNumber, program, functions, inFunc, file)

         
    return program, functions
    
def getStrInBrackets(line: str) -> str:
    return line.split("(")[1].split(")")[0]
 
def isCall(parts: list[str]) -> bool:
    return parts[-1].__contains__("(") or parts[-2].__contains__("(")

def getOperator(parts: list[str]) -> str:
    operator = NO_OPERATION
    
    for part in parts:
        if get_operator(part) != NO_OPERATION:
            operator = get_operator(part)
            break
    return operator

def string_after_equals(parts: list[str]) -> str:
    for index, part in enumerate(parts):
        if part.__contains__("="):
            return ''.join(parts[index:]).replace("=", "").replace(" ", "")
            
    return ""
        
def tokenizeLine(line: str, lineNumber: int, program: list[str], functions: dict[FunctionData], inFunc: list[bool], file: list[str]) -> None:
    line = line.replace(";", " ")
    parts = split_without_brackets(line)
    parts = filter_out_comments(parts)
    
    if(parts.__len__() == 0):
        return
        
    if line.__contains__("input"):
        foo = 0
             
    type: Type = getType(parts[0])
    if type != UNKNOWN:
        
        if parts.__len__() < 2:
            print(f"!!<error> line: {lineNumber+1} only has a type!!")
            exit(1)
        
        callName: str = parts[-1].split("=")[-1].split("(")[0]
        
        if is_variable(parts):
            if isCall(parts):
                parts.pop(2)
                program.extend(parts)

            else:
                operator = getOperator(parts)
                            
                if operator != NO_OPERATION:
                    operation = string_after_equals(parts)
                    values = operation.split(operator)
                    program.append(operator)
                    program.append(parts[0])
                    program.append(values[0])
                    program.append(values[1])
                    parts[-1] = OPERATION_VALUE
                    
                parts.pop(2)
                program.extend(parts)

        
        elif containsBrackets(parts[1]):
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
        
        if parts.__len__() > 1:
            program.append(parts[1].replace(";", ""))
        else: 
            program.append("")
        
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
    
    