#include "debug.h"

DebugData::DebugData()
{
    currentLine = 0;
    commandName = "";
}

void exit_LineHasNoValue(uint32_t lineNumber)
{
    cout << "!!<error> no value given to command line: " << lineNumber + 1 << "!!" << endl;
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

void checkIfTypeIsValid(TkasmType &type, string &rawType, DebugData* data)
{
    if (type == tkasm_unknown)
    {
        cout << "!!<error> type[" << type << "] not a valid type" << stringOfDebugData(data) << endl;
        exit(1);
    }
}

void checkIfStackIsEmpty(Stack* st, DebugData* data)
{
    if (st->size() == 0)
    {
        cout << "!!<error> command called at empty stack " << stringOfDebugData(data) << "!!" << endl;
        exit(1);
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