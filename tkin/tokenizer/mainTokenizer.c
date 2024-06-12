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
#include "../lib/Stream/Stream.h"
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

void* popOrGetType(arraylist/*const char[]*/ *parts, Stream* typeStack, const char *command, const uint32_t i, const uint32_t offsetTypePosition)
{
	if(parts->size > 1+offsetTypePosition)
	{
		const char* potentialType = arraylist_get(parts, 1+offsetTypePosition);
		if(getType(potentialType) != tkasm_unknown)
			return (void*)potentialType;
	}

	const char *type = Stream_pop(typeStack);
	if(getType(type) == tkasm_unknown)
		exit_TypeIsNotValid(type, DebugData_new(command, i));

	return (void*)type;
}

size_t getArraySize(const char* rawArray)
{
	size_t size;
	split_string(rawArray, ",", &size);
	return size;
}

const char* getParsedArray(const char* rawArray)
{
	rawArray = trimWhiteSpaces(rawArray);
	StringStream *stream = StringStream_new();

	ssize_t i = -1;
	while (rawArray[++i] != '\0')
	{
		const char letter = rawArray[i];

		if(letter == '{' || letter == '}')
			continue;

		if(letter == ',')
		{
			StringStream_append(stream, ' ');
			continue;
		}

		StringStream_append(stream, letter);
	}

	const char* result = StringStream_toCharPtr(stream);
	StringStream_free(stream);
	return result;
}

arraylist/*const char[]*/ *tokenizer(arraylist/*const char[]*/ *lines, /*out*/map_int_t *labelTracker, /*out*/map_int_t *lineNumberTracker)
{

	if(lines->size == 0)
	{
		printf("!!<error> tkasm code empty!!");
		exit(1);
	}

	arraylist/*const char[]*/ *tokenLines = arraylist_create();
	Stream/*const char[]*/ *typeStack = Stream_create(NULL);
	Stream/*size_t*/ *arraySizeStack = Stream_create(NULL);

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
			line = trimWhiteSpaces(line);
			size_t size;
			const char **split = split_string((char*)line, ":", &size);
			map_set(labelTracker, split[0], lineNumber);
			free(split);
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
			lineNumber++;
		}
		break;

		case tkasm_pop:
		case tkasm_printPop:
		case tkasm_printPeek:
		{
			if (Stream_size(typeStack) == 0)
				exit_stackIsEmpty(DebugData_new("pop", i + 1));

			const char *type = popOrGetType(parts, typeStack, command, i, 0);
			arraylist_add(tokenLines, (void*)type);

			if(getTypeClass_FromStr(type) == tkasmClass_array)
			{
				const size_t size = (size_t)Stream_pop(arraySizeStack);
				arraylist_add(tokenLines, (void*)uint32_toString(size));
				lineNumber++;
			}

			lineNumber++;
		}
		break;

		case tkasm_read:
		case tkasm_movPush:
		{
			checkIfCommandHasType(parts, i);

			const char *type = arraylist_get(parts, 1);
			arraylist_add(tokenLines, (void*)type);

			if(getTypeClass_FromStr(type) == tkasmClass_array)
			{
				printf("!!<error> you stupid!!");
				exit(1);
			}

			lineNumber++;

			Stream_push(typeStack, (void*)type);
		}
		break;

		case tkasm_shiftLeft:
		case tkasm_shiftRight:
		{
			const char *type = popOrGetType(parts, typeStack, command, i, 0);

			const char *value = arraylist_get(parts, 1);

			arraylist_add(tokenLines, (void*)type);
			arraylist_add(tokenLines, (void*)value);

			lineNumber+=2;

			Stream_push(typeStack, (void*)type);
		}
		break;

		case tkasm_push:
		{
			checkIfCommandHasType(parts, i);

			const char *type = arraylist_get(parts, 1);
			const char *value;

			if(getTypeClass_FromStr(type) == tkasmClass_array)
			{
				const char* rawArray = arraylist_get(parts, 2);
				value = getParsedArray(rawArray);

				void* size = (void*)getArraySize(rawArray);
				Stream_push(arraySizeStack, size);
			}
			else
			{
				value = arraylist_get(parts, 2);
			}

			arraylist_add(tokenLines, (void*)type);
			arraylist_add(tokenLines, (void*)value);

			lineNumber+=2;

			Stream_push(typeStack, (void*)type);
		}
		break;

		case tkasm_add:
		case tkasm_sub:
		case tkasm_mull:
		case tkasm_div:
		{
			if (Stream_size(typeStack) < 2)
				exit_stackIsEmpty(DebugData_new("pop..", i + 1));

			const char *type1 = popOrGetType(parts, typeStack, command, i, 0);
			const char *type2 = popOrGetType(parts, typeStack, command, i, 1);

			arraylist_add(tokenLines, (void*)type1);
			arraylist_add(tokenLines, (void*)type2);

			lineNumber += 2;

			const char *type = BiggerStringType(type1, type2);
			Stream_push(typeStack, (void*)type);
		}
		break;

		case tkasm_movPop:
		case tkasm_jumpEquals0:
		case tkasm_jumpGreater0:
		case tkasm_jumpSmaller0:
		{
			if (parts->size < 1)
				exit_LineHasNoValue(i);

			if (Stream_size(typeStack) < 1)
				exit_stackIsEmpty(DebugData_new("jump..", i + 1));

			const char *type = popOrGetType(parts, typeStack, command, i, 0);

			Stream_push(typeStack, (void*)type);

			const char* value = arraylist_get(parts, 1);

			arraylist_add(tokenLines, (void*)type);
			arraylist_add(tokenLines, (void*)value);

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


			lineNumber++;
		}
		break;

		default:
			break;
		}
		SET_LINETRACKER(lineNumberTracker, tokenLines, i + 1);
	}
    const char *lastCommand = arraylist_get(tokenLines, tokenLines->size - 1);
	if (strcmp(lastCommand, "halt") != 0)
	{
		printf("!!<error> tkasm code does not end with halt!!");
		exit(1);
	}

	Stream_free(typeStack);
	return tokenLines;
}