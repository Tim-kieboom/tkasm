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

#define JUMP_TO_LABEL(hashMap, strLabel) map_get(hashMap, strLabel)

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

	else if (STR_EQUALS(command, "halt"))
	{
		return tkasm_halt;
	}

	printf("!!<error> type not recognized command: ");
	printf(command);
	printf("!!");

	exit(1);
}

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

void tk_pop(/*out*/Stack* stack, const char* rawType, DebugData* data)
{
	const TkasmType type = getType(rawType);
	uint8_t* segments = popType(&type, stack);
	free(segments);
}


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

void tk_print(const char* rawValue, DebugData* data)
{
	const char* message = getInternalString((char*)rawValue);
	message = parse_backslash(message);
	printf(message);
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

	free(segments);
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

void tk_jump(/*out*/uint32_t *index, map_int_t *labelTracker, const char* label)
{
	*index = *JUMP_TO_LABEL(labelTracker, label);
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

	uint8_t* segments = peekType(&type, stack);;

	bool isSuccess;
	void* value = unsegmentType(&type, segments, /*out*/&isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	free(segments);
	return isTypeSmaller0(&type, value);
}