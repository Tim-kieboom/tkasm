from assets.tkasmTokenizer import tokenize
from assets.tkasmCompiler import compile
import os

def execute(dir_path):
    
    print("[CMD] Assembling ...")
    os.system(f"nasm -f win32 \"{dir_path}\\main.asm\" -o \"{dir_path}\\main.o\"")
    
    print("[CMD] Linking ...")
    os.system(f"gcc \"{dir_path}\\main.o\" -o main.exe")
        
    print("[CMD] Running ...")
    os.system(f"main.exe")
        
        
FILE_PATH: str = "main.tkasm"

lines: list[str] = open(FILE_PATH, "r").readlines()

print("\n================= FILE =================\n")  
for line in lines:
    print(line, end="")
print()
    
program: list[str] = tokenize(lines)

print("\n================= TOKENIZED =================\n")
for line in program:
    print(line)
    
compile(program, "compiled")
execute("compiled")

    