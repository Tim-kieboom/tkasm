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
#include "../lib/stringTools/stringTools.h"
#include "../metaData/Commands/commands.h"
#include "../metaData/debug/debug.h"

#define SET_LINETRACKER(hashmap, tokenLines, index)						\
	const char* key = uint32_toString((int)tokenLines->size - 1);		\
	map_set(lineNumberTracker, key, (int)(i+1));

#define IS_COMMAND_STOP_OR_JUMP(lastCommand)\
(											\
	STR_EQUALS(lastCommand, "halt")   ||	\
	STR_EQUALS(lastCommand, "return") ||	\
	STR_EQUALS(lastCommand, "call")   ||	\
	STR_EQUALS(lastCommand, "jump")			\
)

#define IS_VALID_LINE(line) !(STR_EQUALS(line, "\n") || isLineCommand(line))

void getLabelStackSize(TokenizeData *data, uint32_t *popCounter)
{
	if(Stream_isEmpty(data->typeStack))
		return;

	const char* rawType = Stream_peek(data->typeStack);
	const TkasmType type = getType(rawType);
	const uint16_t amountBytes = abs(getTypeSize(&type)) / 8;

	if(type == tkasm_returnPointer)
		return;

	Stream_pop(data->typeStack);
	*popCounter += amountBytes;
}

void getLabelReturnType(arraylist *tokenizeHistory, const char* rawReturnType)
{

}

const char* addLabelTypeWithLineNumber(const char* type, const uint32_t lineNumber)
{
	StringStream *stream = StringStream_new();
	StringStream_appendCharPtr(stream, type);
	StringStream_append(stream, ' ');
	StringStream_appendCharPtr(stream, uint32_toString(lineNumber));

	const char *value = StringStream_toCharPtr(stream);
	StringStream_free(stream);

	return value;
}

TokenizeData *TokenizeData_new()
{
	TokenizeData *data = malloc(sizeof(TokenizeData));
	data->labelTracker = NEW_MAP(map_str_t);
	data->lineNumberTracker = NEW_MAP(map_int_t);
	data->typeStack = Stream_create(NULL);
	data->arraySizeStack = Stream_create(NULL);
	return data;
}

void TokenizeData_free(TokenizeData *data)
{
	if(data == NULL)
		return;

	if(data->labelTracker != NULL)
	{
		map_deinit(data->labelTracker);
		free(data->labelTracker);
	}

	if(data->lineNumberTracker != NULL)
	{
		map_deinit(data->lineNumberTracker);
		free(data->lineNumberTracker);
	}

	if(data->typeStack != NULL)
		Stream_free(data->typeStack);

	if(data->arraySizeStack != NULL)
		Stream_free(data->arraySizeStack);

	free(data);
}


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

void checkIfFileEndOnJumpOrStop(arraylist *lines)
{
	const char* line = "";
	uint32_t index = lines->size -1;
	while(index-- != 0)
	{
		line = arraylist_get(lines, index);
		if(IS_VALID_LINE(line))
			break;
	}

	arraylist *parts = splitWhiteSpaces(line);
	const char* lastcommand = arraylist_get(parts, 0);
	if(!IS_COMMAND_STOP_OR_JUMP(lastcommand))
	{
		printf("!!<error> tkasm file has to end on a halt!!");
		exit(1);
	}
	arraylist_destroy(parts);
}

arraylist/*const char[]*/ *tokenizeLine(const char* line, /*out*/uint32_t *i, /*out*/arraylist *tokenizeHistory, /*out*/const TokenizeData *data);

arraylist/*const char[]*/ *tokenizeFile(arraylist/*const char[]*/ *lines, /*out*/const TokenizeData *tokenizeData)
{

	if(lines->size == 0)
	{
		printf("!!<error> tkasm code empty!!");
		exit(1);
	}

	arraylist *tokenizeHistory = arraylist_create();

	for (uint32_t i = 0; i < lines->size; i++)
	{
		if (tokenizeHistory->size > INT_MAX)
			printf( "!!<warning> tokenLines to big!!");

		const char* line = arraylist_get(lines, i);
		tokenizeLine(line, /*out*/&i, /*out*/tokenizeHistory, /*out*/tokenizeData);
	}

	checkIfFileEndOnJumpOrStop(lines);
	return tokenizeHistory;
}

arraylist/*const char[]*/ *tokenizeLine(const char* line, /*out*/uint32_t *i, /*out*/arraylist *tokenizeHistory, /*out*/const TokenizeData *data)
{
	arraylist *tokenizedLine = arraylist_create();

	if (isLineCommand(line))
		return NULL;

	arraylist/*const char[]*/ *parts = splitWhiteSpaces(line);

	checkForCommands(/*out*/parts);

	if (parts->size == 0)
		return NULL;

	const char* command = arraylist_get(parts, 0);

	//check if line is label
	const char lastChar = line[String_size(line)-1];
	if (lastChar == ':')
	{
		size_t size;
		map_str_t *labelTracker = data->labelTracker;

		const char **split = split_string((char*)line, ":", &size);

        arraylist *labelParts = splitWhiteSpaces(split[0]);

		const char* labelType = (labelParts->size < 2) ? "%void" : trimWhiteSpaces(arraylist_get(labelParts, 0));
		const char *mapValue = addLabelTypeWithLineNumber(labelType, tokenizeHistory->size);

		const char *label = (labelParts->size < 2) ? trimWhiteSpaces(arraylist_get(labelParts, 0)) : arraylist_get(labelParts, 1);

		printf("\nmapset labeltracker: %s, %s\n", label, mapValue);

		map_set(labelTracker, label, mapValue);
		free(split);

		return NULL;
	}

	arraylist_add(tokenizeHistory, (void*)command);
	arraylist_add(tokenizedLine, (void*)command);
	map_set(data->lineNumberTracker, uint32_toString(tokenizeHistory->size) , *i+1); //!!

	const TKasmCommand TkCommand = getCommand(command);

	switch (TkCommand)
	{
	case tkasm_jump:
	{
		const char *label = arraylist_get(parts, 1);
		arraylist_add(tokenizeHistory, (void*)label);
		arraylist_add(tokenizedLine, (void*)label);
	}
	break;


	case tkasm_call:
	{
		const char *label = arraylist_get(parts, 1);
		arraylist_add(tokenizeHistory, (void*)label);
		arraylist_add(tokenizedLine, (void*)label);

		const TkasmType returnAdress = tkasm_returnPointer;
		const char* type = getTypeString(&returnAdress);
		Stream_push(data->typeStack, (void*)type);
	}
	break;

	case tkasm_return:
	{
		size_t popCounter = 0;
		while(true)
		{
			if(Stream_isEmpty(data->typeStack))
				break;

			const char* rawType = Stream_pop(data->typeStack);
			TkasmType type = getType(rawType);
			const uint16_t amountBytes = abs(getTypeSize(&type)) / 8;

			if(type == tkasm_returnPointer)
				break;

			popCounter += amountBytes;
		}

		const char* labelStackSize = uint32_toString(popCounter);
		arraylist_add(tokenizeHistory, (void*)labelStackSize);
		arraylist_add(tokenizedLine, (void*)labelStackSize);

		const char* returnType = (parts->size < 2) ? "%void" : arraylist_get(parts, 1);
		arraylist_add(tokenizeHistory, (void*)returnType);
		arraylist_add(tokenizedLine, (void*)returnType);

		Stream_push(data->typeStack, (void*)returnType);
	}
	break;

	case tkasm_pop:
	case tkasm_printPop:
	case tkasm_printPeek:
	{
		if (Stream_size(data->typeStack) == 0)
			exit_stackIsEmpty(DebugData_new("pop", *i + 1));

		const char *type = popOrGetType(parts, data->typeStack, command, *i, 0);
		arraylist_add(tokenizeHistory, (void*)type);
		arraylist_add(tokenizedLine, (void*)type);

		if(getTypeClass_FromStr(type) == tkasmClass_array)
		{
			const size_t size = (size_t)Stream_pop(data->arraySizeStack);
			arraylist_add(tokenizeHistory, (void*)uint32_toString(size));
			arraylist_add(tokenizedLine, (void*)uint32_toString(size));
		}
	}
	break;

	case tkasm_read:
	case tkasm_movPush:
	{
		checkIfCommandHasType(parts, *i);

		const char *type = arraylist_get(parts, 1);
		arraylist_add(tokenizeHistory, (void*)type);
		arraylist_add(tokenizedLine, (void*)type);

		if(getTypeClass_FromStr(type) == tkasmClass_array)
		{
			printf("!!<error> you stupid!!");
			exit(1);
		}

		Stream_push(data->typeStack, (void*)type);
	}
	break;

	case tkasm_shiftLeft:
	case tkasm_shiftRight:
	{
		const char *type = popOrGetType(parts, data->typeStack, command, *i, 0);

		const char *value = arraylist_get(parts, 1);

		arraylist_add(tokenizeHistory, (void*)type);
		arraylist_add(tokenizeHistory, (void*)value);
		arraylist_add(tokenizedLine, (void*)type);
		arraylist_add(tokenizedLine, (void*)value);

		Stream_push(data->typeStack, (void*)type);
	}
	break;

	case tkasm_push:
	{
		checkIfCommandHasType(parts, *i);

		const char *type = arraylist_get(parts, 1);
		const char *value;

		if(getTypeClass_FromStr(type) == tkasmClass_array)
		{
			const char* rawArray = arraylist_get(parts, 2);
			value = getParsedArray(rawArray);

			void* size = (void*)getArraySize(rawArray);
			Stream_push(data->arraySizeStack, size);
		}
		else
		{
			value = arraylist_get(parts, 2);
		}

		arraylist_add(tokenizeHistory, (void*)type);
		arraylist_add(tokenizeHistory, (void*)value);
		arraylist_add(tokenizedLine, (void*)type);
		arraylist_add(tokenizedLine, (void*)value);

		Stream_push(data->typeStack, (void*)type);
	}
	break;

	case tkasm_add:
	case tkasm_sub:
	case tkasm_mull:
	case tkasm_div:
	{
		if (Stream_size(data->typeStack) < 2)
			exit_stackIsEmpty(DebugData_new("pop..", *i + 1));

		const char *type1 = popOrGetType(parts, data->typeStack, command, *i, 0);
		const char *type2 = popOrGetType(parts, data->typeStack, command, *i, 1);

		arraylist_add(tokenizeHistory, (void*)type1);
		arraylist_add(tokenizeHistory, (void*)type2);
		arraylist_add(tokenizedLine, (void*)type1);
		arraylist_add(tokenizedLine, (void*)type2);

		const char *type = BiggerStringType(type1, type2);
		Stream_push(data->typeStack, (void*)type);
	}
	break;

	case tkasm_movPop:
	case tkasm_jumpEquals0:
	case tkasm_jumpGreater0:
	case tkasm_jumpSmaller0:
	{
		if (parts->size < 1)
			exit_LineHasNoValue(*i);

		if (Stream_size(data->typeStack) < 1)
			exit_stackIsEmpty(DebugData_new("jump..", *i + 1));

		const char *type = popOrGetType(parts, data->typeStack, command, *i, 0);

		Stream_push(data->typeStack, (void*)type);

		const char* value = arraylist_get(parts, 1);

		arraylist_add(tokenizeHistory, (void*)type);
		arraylist_add(tokenizeHistory, (void*)value);
		arraylist_add(tokenizedLine, (void*)type);
		arraylist_add(tokenizedLine, (void*)value);
	}
	break;

	case tkasm_print:
	case tkasm_free:
	{
		if (parts->size < 1)
			exit_LineHasNoValue(*i);

		StringStream *printable = StringStream_new();
		for (uint32_t index = 1; index < parts->size; ++index)
			StringStream_appendCharPtr(printable, arraylist_get(parts, index));

		const char *value = StringStream_toCharPtr(printable);
		StringStream_free(printable);

		arraylist_add(tokenizeHistory, (void*)value);
		arraylist_add(tokenizedLine, (void*)value);
	}
	break;

	default:
		break;
	}
	map_int_t *lineNumberTracker = data->lineNumberTracker;
	SET_LINETRACKER(lineNumberTracker, tokenizeHistory, *i + 1);

	return tokenizedLine;
}