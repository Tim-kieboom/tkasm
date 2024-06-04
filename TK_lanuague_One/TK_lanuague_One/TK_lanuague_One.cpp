#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "fileReader.h"
#include "interpreter.h"

using namespace std;

int main()
{
	const char* programFilePath = "main.tkasm";
	vector<string>* lines = readFile(programFilePath);
	interpretCode(lines);
	delete lines;
}