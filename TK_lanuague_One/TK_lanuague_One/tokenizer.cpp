#include "tokenizer.h"

#include "stringTools.h"

TKasmCommand getCommand(const char* command)
{
    if (STR_EQUALS(command, "push"))
    {
        return tkasm_push;
    }
    else if (STR_EQUALS(command, "pop"))
    {
        return tkasm_pop;
    }
    else if (STR_EQUALS(command, "add"))
    {
        return tkasm_add;
    }
    else if (STR_EQUALS(command, "sub"))
    {
        return tkasm_sub;
    }
    else if (STR_EQUALS(command, "print"))
    {
        return tkasm_print;
    }
    else if (STR_EQUALS(command, "print.pop"))
    {
        return tkasm_printPop;
    }
    else if (STR_EQUALS(command, "read"))
    {
        return tkasm_read;
    }
    else if (STR_EQUALS(command, "jump"))
    {
        return tkasm_jump;
    }
    else if (STR_EQUALS(command, "jump.equals.0"))
    {
        return tkasm_jumpEquals0;
    }
    else if (STR_EQUALS(command, "jump.greater.0"))
    {
        return tkasm_jumpGreater0;
    }
    else if (STR_EQUALS(command, "halt"))
    {
        return tkasm_halt;
    }
}

void checkIfLineHasValue(size_t PartsSize, int32_t lineNumber)
{
    if (PartsSize < 2)
    {
        cout << "!!<error> no value given to command line: " << lineNumber + 1 << "!!" << endl;
        exit(1);
    }
}

void checkIfCommandHasType(vector<string> parts, int32_t lineNumber)
{
    if (parts.size() > 1)
    {
        if (parts[1][0] == '%')
            return;
    }

    cout << "!!<error> no type given to command line: " << lineNumber + 1 << "!!" << endl;
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

void checkIfStackIsEmpty(stack<int32_t>* st, DebugData* data)
{
    if (st->size() == 0)
    {
        cout << "!!<error> command called at empty stack " << stringOfDebugData(data) << "!!" << endl;
        exit(1);
    }
}

vector<string> tokenizer(vector<string>* lines, unordered_map<string, int32_t>& labelTracker, unordered_map<int32_t, int32_t>& lineNumberTracker)
{
    vector<string> tokenLines = vector<string>();

    int32_t lineNumber = 0;
    for (int32_t i = 0; i < lines->size(); i++)
    {
        string line = lines->at(i);

        if (isLineCommand(line))
            continue;

        vector<string> parts = splitWhiteSpaces(line);

        if (parts.size() == 0)
            continue;

        const char* command = parts[0].data();

        //check if line is label
        if (stringLastChar(command) == ':')
        {
            string label = splitString(line, ':')[0];
            labelTracker[label] = lineNumber;
            continue;
        }

        tokenLines.push_back(command);
        lineNumberTracker[tokenLines.size() - 1] = i + 1;
        lineNumber++;

        TKasmCommand TkCommand = getCommand(command);

        switch (TkCommand)
        {
        case tkasm_pop:
        case tkasm_add:
        case tkasm_sub:
        case tkasm_printPop:
        case tkasm_read:
        case tkasm_jump:
        {
            checkIfCommandHasType(parts, i);

            string type = parts[1];
            tokenLines.push_back(type);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;
            lineNumber++;
        }
        break;

        case tkasm_push:
        case tkasm_jumpEquals0:
        case tkasm_jumpGreater0:
        {
            checkIfCommandHasType(parts, i);
            checkIfLineHasValue(parts.size(), i);

            string type = parts[1];
            tokenLines.push_back(type);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            string value = parts[2];
            tokenLines.push_back(value);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            lineNumber++;
            lineNumber++;
        }
        break;


        case tkasm_print:
        {
            checkIfLineHasValue(parts.size() + 1, i);

            string printable;
            for (size_t i = 1; i < parts.size(); ++i)
                printable += parts[i];

            tokenLines.push_back(printable);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;
            lineNumber++;
        }
        break;

        default:
            break;
        }
    }

    return tokenLines;
}