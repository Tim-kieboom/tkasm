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
	const char* programFilePath = "greaterSmallerJump.tkasm";
	vector<string>* lines = readFile(programFilePath);
	interpretCode(lines);
}