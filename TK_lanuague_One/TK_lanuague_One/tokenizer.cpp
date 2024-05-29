#include "tokenizer.h"
#include "stringTools.h"

bool isLineCommand(string line)
{
    if (line.size() < 2)
        return false;

    return line[0] == '/' && line[1] == '/';
}

vector<string> tokenizer(vector<string>* lines, unordered_map<string, uint32_t>& labelTracker, unordered_map<uint32_t, uint32_t>& lineNumberTracker)
{
    vector<string> tokenLines = vector<string>();

    uint32_t lineNumber = 0;
    for (uint32_t i = 0; i < lines->size(); i++)
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
        case tkasm_movPush:
        {
            checkIfCommandHasType(parts, i);

            string type = parts[1];
            tokenLines.push_back(type);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;
            lineNumber++;
        }
        break;

        case tkasm_push:
        case tkasm_movPop:
        case tkasm_jumpEquals0:
        case tkasm_jumpGreater0:
        {
            checkIfCommandHasType(parts, i);
            
            if(parts.size() < 2)
                exit_LineHasNoValue(i);

            string type = parts[1];
            tokenLines.push_back(type);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            string value = parts[2];
            tokenLines.push_back(value);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            lineNumber += 2;
        }
        break;

        case tkasm_mov:
        {
            checkIfCommandHasType(parts, i);
            
            if (parts.size() < 2)
            {
                cout << "!!<error> no variable name given to command line: " << lineNumber + 1 << "!!" << endl;
                exit(1);
            }

            if (parts.size() < 3)
                exit_LineHasNoValue(i);


            string type = parts[1];
            tokenLines.push_back(type);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            string name = parts[2];
            tokenLines.push_back(name);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            string value = parts[3];
            tokenLines.push_back(value);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;

            lineNumber += 3;

        }
        break;

        case tkasm_print:
        case tkasm_free:
        {
            if (parts.size() < 1)
                exit_LineHasNoValue(i);

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