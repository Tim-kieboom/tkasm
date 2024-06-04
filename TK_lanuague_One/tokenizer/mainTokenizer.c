//
// Created by tim_k on 04/06/2024.
//

#include "mainTokenizer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../metaData/Types.h"
#include "../lib/String/String.h"
#include "../lib/HashMap/map.h"
#include "../lib/Stack/Stack.h"
#include "../lib/stringTools/stringTools.h"

typedef enum TkasmType TkasmType;
typedef enum TkasmCommand TkasmCommand;

void checkForCommands(arraylist/*strings*/ *parts)
{
	for (uint16_t i = 0; i < parts->size; i++)
	{
		String *part = arraylist_get(parts, i);
		if (String_size(part) < 2)
			continue;

		if (String_at(part, 0) == '/' && String_at(part, 1) == '/')
		{
			while (String_size(part) > i)
				arraylist_pop(parts);
		}
	}
}

bool isLineCommand(String *line)
{
	if (String_size(line) < 2)
		return false;

	return String_at(line , 0) == '/' && String_at(line , 1) == '/';
}

String *BiggerStringType(String *type1, String *type2)
{
	//TkasmType biggerType = getBiggerType(getType(type1), getType(type2));
	//return String("%") + getTypeString(biggerType);
}

arraylist/*strings*/ *tokenizer(arraylist/*strings*/* lines, /*out*/map_int_t *labelTracker, /*out*/map_int_t *lineNumberTracker)
{

	if(lines->size == 0)
	{
		printf("!!<error> tkasm code empty!!");
		exit(1);
	}

	arraylist/*strings*/ *tokenLines = arraylist_create();

	Stack/*string*/ *typeStack = Stack_create(NULL);

	uint32_t lineNumber = 0;
	for (uint32_t i = 0; i < lines->size(); i++)
	{
		if (tokenLines->size > INT_MAX)
			printf( "!!<warning> tokenLines to big!!");

		String *line = arraylist_get(lines, i);

		if (isLineCommand(line))
			continue;

		arraylist/*strings*/ *parts = splitWhiteSpaces(line);

		checkForCommands(/*out*/parts);

		if (parts->size == 0)
			continue;

		String* command = (String*)arraylist_get(parts, 0);

		//check if line is label
		const char lastChar = String_at(command, String_size(command)-1);
		if (lastChar == ':')
		{
			char **label = (char**)split_string((char*)line->data, ':')[0];
			map_set(labelTracker, label[0], lineNumber);
			continue;
		}

		String_free(line);

		arraylist_add(tokenLines, command);
		map_set(lineNumberTracker, uint32_toString(tokenLines->size - 1) , i+1);
		lineNumber++;

		TKasmCommand TkCommand = getCommand(command);

		switch (TkCommand)
		{
		case tkasm_jump:
		{
			struct string label = parts[1];
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
			if (parts.size() < 1)
				exit_LineHasNoValue(i);

			if (typeStack.size() < 1)
				exit_stackIsEmpty(new DebugData("jump..", i + 1));

			string type = typeStack.top();
			typeStack.pop();
			typeStack.push(type);

			string value = parts[1];

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
    String *lastCommand = arraylist_get(tokenLines, tokenLines->size - 1);
	if (strcmp(lastCommand->data, "halt") != 0)
	{
		printf("!!<error> tkasm code does not end with halt!!");
		exit(1);
	}
	
	String_free(lastCommand);
	return tokenLines;
}