from assets.tkasmTokenizer import tokenize
from assets.tkasmCompiler import compile
from assets.metaData import isCommand
from assets.Types import *

# nasm -f elf64 main.asm -o main.o && ld main.o -o main && ./main

        
FILE_PATH: str = "main.tkasm"

lines: list[str] = open(FILE_PATH, "r").readlines()

print("\n================= FILE =================\n")  
for line in lines:
    print(line, end="")
print()
    
(program, functions) = tokenize(lines)
string_map = compile(program, functions, "compiled")

print("\n================= TOKENIZED =================\n")
inFunc = False
i = 0
while i < len(program):
    line = program[i]
    type = getType(line)
    i += 1
    
    if line == r"%end_func":
        inFunc = False
    
    enter = "\n" if (type != UNKNOWN and type != VOID) or isCommand(line) else ""
    space: str = "\t" if inFunc else ""      
    print(enter + space + str(line))
    
    if line == r"%func":
        print(program[i])
        i+=1
        inFunc = True
    

print("\n================= STRING_MAP =================\n")
for key in string_map.keys():
    print(key, string_map[key])

