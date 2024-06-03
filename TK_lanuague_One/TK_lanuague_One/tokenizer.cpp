#include "tokenizer.h"
#include "stringTools.h"
#include "Commands.h"
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

vector<string> tokenizer(vector<string>* lines, /*out*/unordered_map<string, uint32_t>& labelTracker, /*out*/unordered_map<uint32_t, uint32_t>& lineNumberTracker)
{
	vector<string> tokenLines = vector<string>();

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
		case tkasm_pop:
		case tkasm_printPop:
		case tkasm_read:
		case tkasm_jump:
		case tkasm_movPush:
		{
			checkIfCommandHasType(parts, i);

			string type = parts[1];
			tokenLines.push_back(type);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;
		}
		break;

		case tkasm_add:
		case tkasm_sub:
		case tkasm_push:
		case tkasm_movPop:
		case tkasm_jumpEquals0:
		case tkasm_jumpGreater0:
		case tkasm_jumpSmaller0:
		{
			checkIfCommandHasType(parts, i);

			if (parts.size() < 3)
				exit_LineHasNoValue(i);

			string type = parts[1];
			tokenLines.push_back(type);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

			string value = parts[2];
			tokenLines.push_back(value);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

			lineNumber += 2;
		}
		break;

		case tkasm_mov:
		{
			checkIfCommandHasType(parts, i);

			if (parts.size() < 3)
			{
				cout << "!!<error> no variable name given to command line: " << lineNumber + 1 << "!!" << endl;
				exit(1);
			}

			if (parts.size() < 4)
				exit_LineHasNoValue(i);


			string type = parts[1];
			tokenLines.push_back(type);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

			string name = parts[2];
			tokenLines.push_back(name);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

			string value = parts[3];
			tokenLines.push_back(value);
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;

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
			lineNumberTracker[(int)tokenLines.size() - 1] = i + 1;
			lineNumber++;
		}
		break;

		default:
			break;
		}
	}

	return tokenLines;
}