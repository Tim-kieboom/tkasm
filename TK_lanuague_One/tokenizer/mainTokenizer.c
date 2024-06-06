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
#include "../lib/Stack/Stack.h"
#include "../lib/stringTools/stringTools.h"
#include "../metaData/Commands/commands.h"
#include "../metaData/debug/debug.h"

#define SET_LINETRACKER(hashmap, tokenLines, index)						\
	const char* key = uint32_toString((int)tokenLines->size - 1);		\
	map_set(lineNumberTracker, key, i+1);

void checkForCommands(arraylist/*const char[]*/ *parts)
{
	for (uint16_t i = 0; i < parts->size; i++)
	{
		const char *part = arraylist_get(parts, i);
		if (String_size(part) < 2)
			continue;

		if (part[0] == '/' && part[1] == '/')
		{
			if(i == 0)
				arraylist_clear(parts);

			for(uint32_t j = i; j < parts->size-i; j++)
				arraylist_pop(parts);
		}
	}
}

const char *BiggerStringType(const char *type1, const char *type2)
{
	const TkasmType biggerType = getBiggerType(getType(type1), getType(type2));
	return getTypeString(&biggerType);
}

arraylist/*const char[]*/ *tokenizer(arraylist/*const char[]*/* lines, /*out*/map_int_t *labelTracker, /*out*/map_int_t *lineNumberTracker)
{

	if(lines->size == 0)
	{
		printf("!!<error> tkasm code empty!!");
		exit(1);
	}

	arraylist/*const char[]*/ *tokenLines = arraylist_create();

	Stack/*const char[]*/ *typeStack = Stack_create(NULL);

	uint32_t lineNumber = 0;
	for (uint32_t i = 0; i < lines->size; i++)
	{
		if (tokenLines->size > INT_MAX)
			printf( "!!<warning> tokenLines to big!!");

		const char* line = arraylist_get(lines, i);
		if (isLineCommand(line))
			continue;

		arraylist/*const char[]*/ *parts = splitWhiteSpaces(line);

		checkForCommands(/*out*/parts);

		if (parts->size == 0)
			continue;

		const char* command = arraylist_get(parts, 0);

		//check if line is label
		const char lastChar = command[String_size(command)-1];
		if (lastChar == ':')
		{
			const char *label = split_string((char*)line, ':')[0];
			map_set(labelTracker, label, lineNumber);
			continue;
		}

		arraylist_add(tokenLines, (void*)command);
		map_set(lineNumberTracker, uint32_toString(tokenLines->size - 1) , i+1);
		lineNumber++;

		const TKasmCommand TkCommand = getCommand(command);

		switch (TkCommand)
		{
		case tkasm_jump:
		{
			const char *label = arraylist_get(parts, 1);
			arraylist_add(tokenLines, (void*)label);
			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber++;
		}
		break;

		case tkasm_pop:
		case tkasm_printPop:
		{
			if (Stack_size(typeStack) == 0)
				exit_stackIsEmpty(DebugData_new("pop", i + 1));

			const char *type = Stack_pop(typeStack);

			arraylist_add(tokenLines, (void*)type);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber++;
		}
		break;

		case tkasm_read:
		case tkasm_movPush:
		{
			checkIfCommandHasType(parts, i);

			const char *type = arraylist_get(parts, 1);
			arraylist_add(tokenLines, (void*)type);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber++;

			Stack_push(typeStack, (void*)type);
		}
		break;

		case tkasm_push:
		{
			const char *type = arraylist_get(parts, 1);
			const char *value = arraylist_get(parts, 2);

			arraylist_add(tokenLines, (void*)type);
			arraylist_add(tokenLines, (void*)value);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber+=2;

			Stack_push(typeStack, (void*)type);
		}
		break;

		case tkasm_add:
		case tkasm_sub:
		case tkasm_mull:
		case tkasm_div:
		{
			if (Stack_size(typeStack) < 2)
				exit_stackIsEmpty(DebugData_new("pop..", i + 1));

			const char *type1 = Stack_pop(typeStack);
			const char *type2 = Stack_pop(typeStack);

			arraylist_add(tokenLines, (void*)type1);
			arraylist_add(tokenLines, (void*)type2);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber += 2;

			const char *type = BiggerStringType(type1, type2);
			Stack_push(typeStack, (void*)type);
		}
		break;

		case tkasm_movPop:
		case tkasm_jumpEquals0:
		case tkasm_jumpGreater0:
		case tkasm_jumpSmaller0:
		{
			if (parts->size < 1)
				exit_LineHasNoValue(i);

			if (Stack_size(typeStack) < 1)
				exit_stackIsEmpty(DebugData_new("jump..", i + 1));

			const char *type = Stack_pop(typeStack);
			Stack_push(typeStack, (void*)type);

			const char* value = arraylist_get(parts, 1);

			arraylist_add(tokenLines, (void*)type);
			arraylist_add(tokenLines, (void*)value);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber += 2;
		}
		break;

		case tkasm_print:
		case tkasm_free:
		{
			if (parts->size < 1)
				exit_LineHasNoValue(i);

			StringStream *printable = StringStream_new();
			for (uint32_t index = 1; index < parts->size; ++index)
				StringStream_appendCharPtr(printable, arraylist_get(parts, index));

			const char *value = StringStream_toCharPtr(printable);
			StringStream_free(printable);

			arraylist_add(tokenLines, (void*)value);

			SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
			lineNumber++;
		}
		break;

		default:
			break;
		}
	}
    const char *lastCommand = arraylist_get(tokenLines, tokenLines->size - 1);
	if (strcmp(lastCommand, "halt") != 0)
	{
		printf("!!<error> tkasm code does not end with halt!!");
		exit(1);
	}

	Stack_free(typeStack);
	return tokenLines;
}