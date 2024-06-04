#include "debug.h"

DebugData::DebugData()
{
	currentLine = 0;
	commandName = "";
}

DebugData::DebugData(string commandName, int32_t currentLine)
{
	this->currentLine = currentLine;
	this->commandName = commandName;
}

void exit_LineHasNoValue(uint32_t lineNumber)
{
	cout << "!!<error> no value given to command line: " << lineNumber + 1 << "!!" << endl;
	exit(1);
}

void exit_stackIsEmpty(DebugData* data)
{
	cout << "!!<error> command called at empty stack " << stringOfDebugData(data) << "!!" << endl;
	exit(1);
}

void exit_TypeIsNotValid(string& rawType, DebugData* data)
{
	cout << "!!<error> type[" << rawType << "] not a valid type" << stringOfDebugData(data) << "!!" << endl;
	exit(1);
}

void exit_SegmentationFailed(string& rawType, DebugData* data)
{
	cout << "!!<error> type[" << rawType << "] has a segmentation error" << stringOfDebugData(data) << "!!" << endl;
	exit(1);
}

void exit_CastingFailed(string& rawType, DebugData* data)
{
	cout << "!!<error> type[" << rawType << "] has a typeCasting error" << stringOfDebugData(data) << "!!" << endl;
	exit(1);
}

bool isLineCommand(string line)
{
	if (line.size() < 2)
		return false;

	return line[0] == '/' && line[1] == '/';
}

string stringOfDebugData(DebugData* data)
{
	return "[line: " + to_string(data->currentLine) + ", command: " + data->commandName + "]";
}

void checkIfStackIsEmpty(stack<uint8_t>* st, DebugData* data)
{
	if (st->size() == 0)
	{
		exit_stackIsEmpty(data);
	}
}

void checkIfCommandHasType(vector<string> parts, uint32_t lineNumber)
{
	if (parts.size() > 1)
	{
		if (parts[1][0] == '%')
			return;
	}

	cout << "!!<error> no type given to command line: " << lineNumber + 1 << "!!" << endl;
}