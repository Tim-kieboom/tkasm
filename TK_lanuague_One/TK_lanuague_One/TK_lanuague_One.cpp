#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "fileReader.h"
#include "interpreter.h"
#include <unordered_map>

/*
instructions needed:
    push:                   push stack
    pop:                    pop stack

    add:                    pop 2 numbers from the stack and sum push the sum
    sub:                    pop 2 numbers and push the subtraction

    print:                  print string_literal to terminal
    read:                   read number from io input and push it to stack

    jump.equals.0 (label):  jump to label if top of stack is 0
    jump.greater.0 (label): jump to label if top of stack is greater then 0

    halt:                   stop program
*/

using namespace std;

int main()
{

    const char* programFilePath = R"(C:\Users\tim_k\OneDrive\Bureaublad\tim\trainProgramming\project\TK_lanuague_One\TK_lanuague_One\testCode.tkasm)";

    vector<string>* lines = readFile(programFilePath);

    interpretCode(lines);
}