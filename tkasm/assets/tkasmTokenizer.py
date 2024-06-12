from assets.command import Commands
from assets.Types import Type, Types

def split_white_spaces(line: str) -> list[str]:
    return line.strip().split()

def is_line_label(parts: list[str]) -> bool:
    return parts[-1][-1] == ':'

def is_line_command(line: str) -> bool:
    return line.strip()[0:2] == "//"

def pop_comments_out_parts(parts: list[str]) -> list[str]:
    
    for index, part in enumerate(parts): 
        if part[0:2] == "//":
            return parts[0:index]

    return parts            

def pop_or_get_type(parts, typeStack, typePosition) -> Type:
    
    if len(parts) > typePosition:
        return Types.getType(parts[typePosition])
    
    if len(typeStack) > 0:
        return typeStack.pop()
    
    return Types.UNKNOWN

def tokenize(lines: list[str]):
    
    program: list[str] = []
    
    typeStack: list[Type] = []
    arraySizeStack: list[int] = []

    label_tracker: dict[str, int] = {}
    line_tracker: dict[int, int] = {}

    for index, line in enumerate(lines):
        
        if is_line_command(line):
            continue
        
        parts: list[str] = split_white_spaces(line)
        parts = pop_comments_out_parts(parts)
        if(len(parts) == 0):
            continue
        
        if(is_line_label(parts)):
            label = parts[-1][:-1]
            label_tracker[label] = len(program)
            continue
        
        command = parts[0]
        line_tracker[len(program)] = index+1
        
        program.append(command)
        
        if command == Commands.PUSH.value:
            # checkType
            # checkValue
            type: Type = Types.getType(parts[1])
            typeStack.append(type)
            
            program.append(type.value.value)
            
            if type.value.isArray:
                # parse array
                continue
            
            value = parts[2]
            
            program.append(value)
            
        elif command == Commands.POP.value or \
            command == Commands.PRINT_POP.value or \
            command == Commands.PRINT_PEEK.value:
            
            if len(typeStack) <= 0:
                print("no type in stack") # make debug func
                exit(1)

            type: Type = pop_or_get_type(parts, typeStack, 1)
            program.append(type.value.value)
            
            if type.value.isArray:
                arraySize: int = arraySizeStack.pop()
                program.append(str(arraySize))
                continue
            
        elif command == Commands.MOV_PUSH.value or \
            command == Commands.READ.value:
            # checkType
            type: Type = Types.getType(parts[1])
            program.append(type.value)
            
            if type.value.isArray:
                # array not implemented
                exit(1)
            
            typeStack.append(type)
            
        elif command == Commands.MOV_POP.value or \
            command == Commands.JUMP_EQUALS_0.value or \
            command == Commands.JUMP_GREATER_0.value or \
            command == Commands.JUMP_SMALLER_0.value:
            # checkValue
            type: Type = pop_or_get_type(parts, typeStack, 1)
            program.append(type.value.value)
            
            if type.value.isArray:
                # array not implemented
                exit(1)
                
            
        elif command == Commands.MOV.value:
            print("!!not implemented!!")
            exit(1)
            
        elif command == Commands.FREE.value or \
            command == Commands.PRINT.value:
            # checkValue
            printable: str = ' '.join( parts[1:len(parts)+1] )
            program.append(printable)
            
        elif command == Commands.ADD.value or \
            command == Commands.SUB.value or \
            command == Commands.MUL.value or \
            command == Commands.DIV.value:
            # checkValue
            type1: Type = pop_or_get_type(parts, typeStack, 1)
            type2: Type = pop_or_get_type(parts, typeStack, 2)
            program.append(type1.value.value)
            program.append(type2.value.value)
            
            bestType: Type = Types.bestType(type1.value, type2.value)
            typeStack.append(bestType)
            
        elif command == Commands.SHIFT_LEFT.value or \
            command == Commands.SHIFT_RIGHT.value:
            
            type: Type = pop_or_get_type(parts, typeStack, 1)
            amount = parts[2]
            
            program.append(type.value.value)
            program.append(amount)
            
            typeStack.append(type)
                
            
        elif command == Commands.JUMP.value:
            label: str = parts[1]
            program.append(command)
            program.append(label)
            
    return program
            
if __name__ == "__main__":
    lines = []
    lines.append(r"push %int8 5")
    lines.append(r"pop")
    lines.append(r"print \"{test print}\"")
    lines.append(r"push %uint32 5")
    lines.append(r"push %int8 5")
    lines.append(r"add")
    lines.append(r"print.pop")
    lines.append(r"halt")
    
    program = tokenize(lines)
    for line in program:
        print(line)
    
    
    