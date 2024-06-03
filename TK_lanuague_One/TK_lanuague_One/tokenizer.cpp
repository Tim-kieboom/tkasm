#include "tokenizer.h"
#include "stringTools.h"
#include "Commands.h"
#include "Types.h"
#include "debug.h"

void checkForCommands(/*out*/vector<string>& parts)
{
	for (uint16_t i = 0; i < parts.size(); i++)
	{
		string part = parts[i];
		if (part.size() < 2)
			continue;

		if (part[0] == '/' && part[1] == '/')
		{
			while (parts.size() > i)
				parts.pop_back();
		}
	}
}

string BiggerStringType(string& type1, string& type2)
{
	TkasmType t1 = getType(type1);
	TkasmType t2 = getType(type2);
	TkasmType biggerType = getBiggerType(t1, t2);
	string type = string("%") + getTypeString(biggerType);
	return type;
}

vector<string> tokenizer(vector<string>* lines, /*out*/unordered_map<string, uint32_t>& labelTracker, /*out*/unordered_map<uint32_t, uint32_t>& lineNumberTracker)
{
	if(lines->size() == 0)
	{
		cout << "!!<error> tkasm code empty!!" << endl;
		exit(1);
	}

	vector<string> tokenLines = vector<string>();
	stack<string> typeStack = stack<string>();

	uint32_t lineNumber = 0;
	for (uint32_t i = 0; i < lines->size(); i++)
	{
		if (tokenLines.size() > INT_MAX)
			cout << "!!<warning> tokenLines to big!!" << endl;

		string line = lines->at(i);

		if (isLineCommand(line))
			continue;

		vector<string> parts = splitWhiteSpaces(line);

		checkForCommands(/*out*/parts);

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
		lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
		lineNumber++;

		TKasmCommand TkCommand = getCommand(command);

		switch (TkCommand)
		{
		case tkasm_jump:
		{
			checkIfCommandHasType(parts, i);

			string label = parts[1];
			tokenLines.push_back(label);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;
		}
		break;

		case tkasm_pop:
		case tkasm_printPop:
		{
			if (typeStack.size() == 0)
				exit_stackIsEmpty(new DebugData("pop", i + 1));

			string type = typeStack.top();
			typeStack.pop();

			tokenLines.push_back(type);

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;

		}
		break;

		case tkasm_read:
		case tkasm_movPush:
		{
			checkIfCommandHasType(parts, i);

			string type = parts[1];
			tokenLines.push_back(type);

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;

			typeStack.push(type);
		}
		break;

		case tkasm_push:
		{
			string type = parts[1];
			string value = parts[2];

			tokenLines.push_back(type);

			tokenLines.push_back(value);

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber+=2;

			typeStack.push(type);
		}
		break;

		case tkasm_add:
		case tkasm_sub:
		case tkasm_mull:
		case tkasm_div:
		{
			if (typeStack.size() < 2)
				exit_stackIsEmpty(new DebugData("pop..", i + 1));

			string type1 = typeStack.top();
			typeStack.pop();
			string type2 = typeStack.top();
			typeStack.pop();

			tokenLines.push_back(type1);

			tokenLines.push_back(type2);

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;			
			lineNumber += 2;

			string type = BiggerStringType(type1, type2);
			typeStack.push(type);
		}
		break;

		case tkasm_movPop:
		case tkasm_jumpEquals0:
		case tkasm_jumpGreater0:
		case tkasm_jumpSmaller0:
		{
			checkIfCommandHasType(parts, i);

			if (parts.size() < 3)
				exit_LineHasNoValue(i);

			if (typeStack.size() < 2)
				exit_stackIsEmpty(new DebugData("jump..", i + 1));

			string type = typeStack.top();
			typeStack.pop();

			string value = parts[2];

			tokenLines.push_back(type);

			tokenLines.push_back(value);

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

			lineNumber += 2;
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

			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;
		}
		break;

		default:
			break;
		}
	}

	if (tokenLines[tokenLines.size() - 1] != "halt")
	{
		cout << "!!<error> tkasm code does not end with halt!!" << endl;
		exit(1);
	}
	return tokenLines;
}