from assets.tkasmTokenizer import tokenize
from assets.tkasmCompiler import compile
import os

def execute(dir_path):
    
    print(f"[CMD] Assembling ... (\"{dir_path}\\printHelloWorld.asm\" -> \"{dir_path}\\main.o\")")
    os.system(f"nasm -f win32 \"{dir_path}\\printHelloWorld.asm\" -o \"{dir_path}\\main.o\"")
    
    print(f"[CMD] Linking ... (\"{dir_path}\\main.o\" -> main.exe)")
    os.system(f"gcc -o main.exe \"{dir_path}\\main.o\" -mingw32")
    
        
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

    