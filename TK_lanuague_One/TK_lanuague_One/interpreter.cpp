#include "interpreter.h"

#define STR_EQUALS(str1, str2) strcmp(str1, str2) == 0
#define JUMP_TO_LABEL(hashMap, label) hashMap[label] 

typedef struct DebugData
{
    int32_t currentLine = 0;
    string commandName = "";
};

void checkIfLineHasValue(size_t PartsSize, int32_t lineNumber)
{
    if (PartsSize < 1)
    {
        cout << "!!<error>" << " no value given to command line: " << lineNumber + 1 << "!!" << endl;
        exit(1);
    }
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

void checkIfStackIsEmpty(stack<int32_t> &st, DebugData *data)
{
    if (st.size() == 0)
    {
        cout << "!!<error> command called at empty stack " << stringOfDebugData(data)  << "!!" << endl;
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

        if(isLineCommand(line))
            continue;

        vector<string> parts = splitString(line, ' ');
        
        if (parts.size() == 0)
            continue;

        const char* command = parts[0].data();

        //check if line is label
        if(stringLastChar(command) == ':')
        {
            string label = splitString(line, ':')[0];
            labelTracker[label] = lineNumber;
            continue;
        }

        tokenLines.push_back(command);
        lineNumberTracker[tokenLines.size() - 1] = i + 1;
        lineNumber++;

        if(STR_EQUALS(command, "push"))
        {
            checkIfLineHasValue(parts.size(), i);

            tokenLines.push_back(parts[1]);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;
            lineNumber++;
        }
        else if (STR_EQUALS(command, "print"))
        {
            checkIfLineHasValue(parts.size(), i);

            string printable;
            for (std::size_t i = 1; i < parts.size(); ++i) 
                printable += parts[i];

            tokenLines.push_back(printable);
            lineNumberTracker[tokenLines.size() - 1] = i+1;
            lineNumber++;
        }
        else if (STR_EQUALS(command, "jump.equals.0") || STR_EQUALS(command, "jump.greater.0"))
        {
            checkIfLineHasValue(parts.size(), i);

            string label = parts[1];

            tokenLines.push_back(label);
            lineNumberTracker[tokenLines.size() - 1] = i + 1;
            lineNumber++;
        }
    }

    return tokenLines;
}

void interpretCode(vector<string>* lines)
{
    auto memStack = stack<int32_t>();

    auto labelTracker = unordered_map<string, int32_t>();
    auto lineNumberTracker = unordered_map<int32_t, int32_t>();
    vector<string> program = tokenizer(lines, /*out*/labelTracker, /*out*/lineNumberTracker);

    size_t i = 0;
    auto debugData = new DebugData();

    while (strcmp(program[i].data(), "halt") != 0)
    {
        const char* command = program[i].data();
        debugData->commandName = command;
        i++;
        debugData->currentLine = lineNumberTracker[i];

        if (STR_EQUALS(command, "push"))
        {
            int32_t value = stringTo_int32(program[i].data());
            i++;
            memStack.push(value);
        }
        else if (STR_EQUALS(command, "pop"))
        {
            checkIfStackIsEmpty(memStack, debugData);
            memStack.pop();
        }
        else if (STR_EQUALS(command, "add"))
        {
            checkIfStackIsEmpty(memStack, debugData);
            int32_t first = memStack.top();
            memStack.pop();

            checkIfStackIsEmpty(memStack, debugData);
            int32_t second = memStack.top();
            memStack.pop();

            memStack.push(first + second);
        }
        else if (STR_EQUALS(command, "sub"))
        {

            checkIfStackIsEmpty(memStack, debugData);
            int32_t first = memStack.top();
            memStack.pop();

            checkIfStackIsEmpty(memStack, debugData);
            int32_t second = memStack.top();
            memStack.pop();

            memStack.push(second - first);
        }
        else if (STR_EQUALS(command, "print"))
        {
            string text = program[i];
            i++;
            cout << text << endl;
        }
        else if (STR_EQUALS(command, "read"))
        {
            int32_t input;
            cin >> input;

            memStack.push(input);
        }
        else if (STR_EQUALS(command, "jump.equals.0"))
        {
            checkIfStackIsEmpty(memStack, debugData);
            int32_t number = memStack.top();

            if (number == 0)
            {
                i = labelTracker[program[i]];
            }
            else
            {
                i++;
            }
        }
        else if (STR_EQUALS(command, "jump.greater.0"))
        {
            checkIfStackIsEmpty(memStack, debugData);
            int32_t number = memStack.top();

            if (number > 0)
            {
                i = JUMP_TO_LABEL(labelTracker, program[i]);
            }
            else
            {
                i++;
            }
        }
    }

    delete debugData;
}