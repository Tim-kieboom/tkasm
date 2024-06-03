#include "fileReader.h"

#include <iostream>
#include <fstream>

vector<string>* readFile(const char* filePath)
{
	return readFile((char*)filePath);
}

vector<string>* readFile(char* filePath)
{
	auto lines = new vector<string>();

	ifstream stream(filePath);
	if (!stream.is_open())
	{
		cout << "file couldn't be found";
		exit(1);
	}

	string line;
	while (getline(stream, line))
		lines->push_back(line);

	stream.close();

	return lines;
}