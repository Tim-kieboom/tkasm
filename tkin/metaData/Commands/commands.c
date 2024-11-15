//
// Created by tim_k on 05/06/2024.
//

#include "commands.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../../lib/HashMap/map.h"
#include "../../lib/stringTools/stringTools.h"
#include "../debug/debug.h"
#include "../Types.h"

void exit_type1and2NotValid(const char* type1, const char* type2 , const DebugData *data)
{
	StringStream *stream = StringStream_new();
	StringStream_appendCharPtr(stream, type1);
	StringStream_appendCharPtr(stream, " and");
	StringStream_appendCharPtr(stream, type2);
	const char* message = StringStream_toCharPtr(stream);
	StringStream_free(stream);

	exit_TypeIsNotValid(message, data);
}

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

	else if (STR_EQUALS(command, "mov.push"))
	{
		return tkasm_movPush;
	}
	else if (STR_EQUALS(command, "mov.pop"))
	{
		return tkasm_movPop;
	}
	else if (STR_EQUALS(command, "mov"))
	{
		return tkasm_mov;
	}

	else if (STR_EQUALS(command, "free"))
	{
		return tkasm_free;
	}

	else if (STR_EQUALS(command, "add"))
	{
		return tkasm_add;
	}
	else if (STR_EQUALS(command, "sub"))
	{
		return tkasm_sub;
	}

	else if (STR_EQUALS(command, "mull"))
	{
		return tkasm_mull;
	}
	else if (STR_EQUALS(command, "div"))
	{
		return tkasm_div;
	}

	else if (STR_EQUALS(command, "shift.left"))
	{
		return tkasm_shiftLeft;
	}
	else if (STR_EQUALS(command, "shift.right"))
	{
		return tkasm_shiftRight;
	}

	else if (STR_EQUALS(command, "print"))
	{
		return tkasm_print;
	}
	else if (STR_EQUALS(command, "print.pop"))
	{
		return tkasm_printPop;
	}
	else if (STR_EQUALS(command, "print.peek"))
	{
		return tkasm_printPeek;
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
	else if (STR_EQUALS(command, "jump.smaller.0"))
	{
		return tkasm_jumpSmaller0;
	}

	else if (STR_EQUALS(command, "call"))
	{
		return tkasm_call;
	}
	else if (STR_EQUALS(command, "return"))
	{
		return tkasm_return;
	}

	else if (STR_EQUALS(command, "halt"))
	{
		return tkasm_halt;
	}

	printf("!!<error> tkasm_command not recognized command: \'%s\'!!", command);

	exit(1);
}

//============================ STACK MEMORY ============================

void tk_push(/*out*/Stack* stack, const char* rawType, const char* rawValue, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	bool isSuccess;
	void* value = stringToType(&type, rawValue, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint8_t* segments = segmentType(&type, value);
	pushType(&type, segments, stack);

	free(segments);
}

void tk_pushArray(/*out*/Stack* stack, const char* rawType, const char* rawValue, const DebugData* data)
{
	const TkasmType type = getTypeOfArray(rawType);

	size_t valuesSize;
	const char** rawValues = split_string(rawValue, " ", /*out*/&valuesSize);
	for (size_t i = valuesSize; i --> 0;)
	{
		bool isSuccess;
		void* value = stringToType(&type, rawValues[i], /*out*/&isSuccess);
		if (!isSuccess)
			exit_TypeIsNotValid(rawType, data);

		uint8_t* segments = segmentType(&type, value);
		pushType(&type, segments, stack);
		free(segments);
	}
}

void tk_pop(/*out*/Stack* stack, const char* rawType, DebugData* data)
{
	const TkasmType type = getType(rawType);
	uint8_t* segments = popType(&type, stack);
	free(segments);
}


//============================ OPERATORS ============================


void tk_add(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data)
{
	const TkasmType type1 = getType(rawType1);
	const TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = addTypes(type1, type2, /*out*/stack, /*out*/&isSuccess, data);
	if (!isSuccess)
		exit_type1and2NotValid(rawType1, rawType2, data);

	const TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(&type, value);

	pushType(&type, segments, stack);
	free(segments);
}

void tk_sub(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data)
{
	const TkasmType type1 = getType(rawType1);
	const TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = subTypes(type1, type2, stack, /*out*/&isSuccess, data);
	if (!isSuccess)
		exit_type1and2NotValid(rawType1, rawType2, data);

	const TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(&type, value);

	pushType(&type, segments, stack);
	free(segments);
}

void tk_mull(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data)
{
	const TkasmType type1 = getType(rawType1);
	const TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = mullTypes(type1, type2, stack, /*out*/&isSuccess, data);
	if (!isSuccess)
		exit_type1and2NotValid(rawType1, rawType2, data);

	const TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(&type, value);

	pushType(&type, segments, stack);
	free(segments);
}

void tk_div(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data)
{
	const TkasmType type1 = getType(rawType1);
	const TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = divTypes(type1, type2, stack, /*out*/&isSuccess, data);
	if (!isSuccess)
		exit_type1and2NotValid(rawType1, rawType2, data);

	const TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(&type, value);

	pushType(&type, segments, stack);
	free(segments);
}

void tk_shiftLeft(/*out*/Stack* stack, const char* rawType, const char* rawValue, const DebugData* data)
{
	const TkasmType type = getType(rawType);
	const uint32_t amount = atoi(rawValue);

	uint8_t* segments = popType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if(!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint64_t uvalue = (uint64_t)value;
	uvalue <<= amount;

	uint8_t* pushSegments = segmentType(&type, (void*)uvalue);
	pushType(&type, pushSegments, stack);
	free(segments);
	free(pushSegments);
}

void tk_shiftRight(/*out*/Stack* stack, const char* rawType, const char* rawValue, const DebugData* data)
{
	const TkasmType type = getType(rawType);
	const uint32_t amount = atoi(rawValue);

	uint8_t* segments = popType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if(!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint64_t uvalue = (uint64_t)value;
	uvalue >>= amount;

	uint8_t* pushSegments = segmentType(&type, (void*)uvalue);
	pushType(&type, pushSegments, stack);
	free(segments);
	free(pushSegments);
}

//============================ PRINTS/READS ============================

void tk_print(const char* rawValue, DebugData* data)
{
	if(!isInternalString(rawValue))
	{
		printf("!!<error> %s is not a string\n", rawValue);
		exit(1);
	}

	const char* message = getInternalString((char*)rawValue);
	message = parse_backslash(message);
	printf("%s\n", message);
}

void tk_printPop(/*out*/Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	uint8_t* segments = popType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	printTypeToConsole(&type, value);

	printf("\n");
	free(segments);
}

void tk_printPopArray(/*out*/Stack* stack, const char* rawType, const char* arraySize, const DebugData* data)
{
	const TkasmTypeClass class = getTypeClass_FromStr(rawType);
	if(class != tkasmClass_array)
		exit_TypeIsNotValid(rawType, data);

	const TkasmType type = getTypeOfArray(rawType);
	const uint32_t size = atoi(arraySize);

	printf("{");
	for(uint32_t i = 0; i < size; i++)
	{
		uint8_t* segments = popType(&type, stack);

		bool isSuccess;
		void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
		printTypeToConsole(&type, value);
		free(segments);

		if(i < size-1)
			printf(", ");
	}
	printf("}");
	printf("\n");

}

void tk_printPeek(const Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	uint8_t* segments = peekType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	printTypeToConsole(&type, value);

	free(segments);
}

void tk_printPeekArray(const Stack* stack, const char* rawType, const char* arraySize, const DebugData* data)
{
	const TkasmTypeClass class = getTypeClass_FromStr(rawType);
	if(class != tkasmClass_array)
		exit_TypeIsNotValid(rawType, data);

	const TkasmType type = getTypeOfArray(rawType);
	const uint32_t size = atoi(arraySize);

	printf("{");
	for(uint32_t i = 0; i < size; i++)
	{
		uint8_t* segments = peekArrayIndexType(&type, stack, i);

		bool isSuccess;
		void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
		printTypeToConsole(&type, value);
		free(segments);

		if(i < size-1)
			printf(", ");
	}
	printf("}");

}

void tk_printPeekArrayIndex(const Stack* stack, const char* rawType, const char* index, const DebugData* data)
{
	const TkasmTypeClass class = getTypeClass_FromStr(rawType);
	if(class != tkasmClass_array)
		exit_TypeIsNotValid(rawType, data);

	const TkasmType type = getTypeOfArray(rawType);
	const uint32_t i = atoi(index);

	uint8_t* segments = peekArrayIndexType(&type, stack, i);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	printTypeToConsole(&type, value);
	free(segments);
}

void tk_read(/*out*/Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	bool isSuccess = false;
	void* value = readTypeFromConsole(&type, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint8_t* segments = segmentType(&type, value);
	pushType(&type, segments, stack);

	free(segments);
}

//============================ JUMPS/LOGIC ============================

void tk_jump(/*out*/uint32_t *index, map_int_t *labelTracker, const char* label)
{
	*index = (uint32_t)*map_get(labelTracker, label);
}

bool tk_isEquals0(const Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	uint8_t* segments = peekType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	free(segments);
	return isTypeEqual0(&type, value);
}

bool tk_isGreater0(const Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	uint8_t* segments = peekType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	free(segments);
	return isTypeGreater0(&type, value);
}

bool tk_isSmaller0(const Stack* stack, const char* rawType, const DebugData* data)
{
	const TkasmType type = getType(rawType);

	uint8_t* segments = peekType(&type, stack);

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	free(segments);
	return isTypeSmaller0(&type, value);
}

void tk_call(/*out*/uint32_t *index, /*out*/Stack *stack, map_int_t *labelTracker, const char* label)
{
	const TkasmType returnType = tkasm_returnPointer;
	printf("\nindex: %d\n", (int64_t)*index);
	uint8_t* segments = segmentType(&returnType, (void*)(int64_t)*index);
	pushType(&returnType, segments, stack);

	*index = (uint32_t)*map_get(labelTracker, label);
	free(segments);
}

void tk_return(/*out*/uint32_t *index, Stack* stack, const char* amountOfBytesToPop, const char* rawType, const DebugData* data)
{
	bool isSuccess;
	size_t amountBytes = _atoi64(amountOfBytesToPop) -1;
	TkasmType returnType = tkasm_unknown;
	uint8_t* returnValue = NULL;
	if(rawType != NULL)
	{
		returnType = getType(rawType);
		returnValue = popType(&returnType, stack);

		amountBytes -= abs(getTypeSize(&returnType))/8;
	}

	removeAmountStack(amountBytes, stack);

	const TkasmType returnPtr = tkasm_returnPointer;
	const uint8_t* segments = popType(&returnPtr, stack);
	const int64_t callAdress = (int64_t)unsegmentType(&returnPtr, segments, &isSuccess);

	printf("return jump from %d to %d", *index, (int)(callAdress + 2));
	*index = callAdress+2;

	if(returnType != tkasm_unknown)
		pushType(&returnType, returnValue, stack);

	if(returnValue != NULL)
		free(returnValue);
}
