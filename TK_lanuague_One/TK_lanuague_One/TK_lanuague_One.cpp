#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "fileReader.h"
#include "interpreter.h"
#include "stringTools.h"



using namespace std;

int main()
{
    const char* programFilePath = "testCode.tkasm";

    vector<string>* lines = readFile(programFilePath);

    interpretCode(lines);
}