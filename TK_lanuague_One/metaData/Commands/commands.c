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

#define JUMP_TO_LABEL(hashMap, strLabel) (int)map_get(hashMap, strLabel)

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

	else if (STR_EQUALS(command, "print"))
	{
		return tkasm_print;
	}
	else if (STR_EQUALS(command, "print.pop"))
	{
		return tkasm_printPop;
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

// void push(/*out*/Stack* stack, const char* rawType, const char* rawValue, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
//
// 	bool isSuccess;
// 	void* value = stringToType(type, rawValue, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	uint8_t* segments = segmentType(type, value);
// 	pushType(type, segments, stack);
//
// 	free(segments);
// }
//
// void pop(/*out*/Stack* stack, const char* rawType, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
// 	uint8_t* segments = popType(type, stack);
// 	free(segments);
// }
//
//
// void add(/*out*/Stack* stack, const char* rawType1, String rawType2, DebugData* data)
// {
// 	TkasmType type1 = getType(rawType1);
// 	TkasmType type2 = getType(rawType2);
//
// 	bool isSuccess;
// 	void* value = addTypes(type1, type2, /*out*/stack, /*out*/isSuccess, data);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);
//
// 	TkasmType type = getBiggerType(type1, type2);
// 	uint8_t* segments = segmentType(type, value);
//
// 	pushType(type, segments, stack);
// 	free(segments);
// }
//
// void sub(/*out*/Stack* stack, const char* rawType1, const char* rawType2, DebugData* data)
// {
// 	TkasmType type1 = getType(rawType1);
// 	TkasmType type2 = getType(rawType2);
//
// 	bool isSuccess;
// 	void* value = subTypes(type1, type2, stack, /*out*/isSuccess, data);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);
//
// 	TkasmType type = getBiggerType(type1, type2);
// 	uint8_t* segments = segmentType(type, value);
//
// 	pushType(type, segments, stack);
// 	free(segments);
// }
//
// void mull(/*out*/Stack* stack, const char* rawType1, const char* rawType2, DebugData* data)
// {
// 	TkasmType type1 = getType(rawType1);
// 	TkasmType type2 = getType(rawType2);
//
// 	bool isSuccess;
// 	void* value = mullTypes(type1, type2, stack, /*out*/isSuccess, data);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);
//
// 	TkasmType type = getBiggerType(type1, type2);
// 	uint8_t* segments = segmentType(type, value);
//
// 	pushType(type, segments, stack);
// 	free(segments);
// }
//
// void div(/*out*/Stack* stack, const char* rawType1, const char* rawType2, DebugData* data)
// {
// 	TkasmType type1 = getType(rawType1);
// 	TkasmType type2 = getType(rawType2);
//
// 	bool isSuccess;
// 	void* value = divTypes(type1, type2, stack, /*out*/isSuccess, data);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);
//
// 	TkasmType type = getBiggerType(type1, type2);
// 	uint8_t* segments = segmentType(type, value);
//
// 	pushType(type, segments, stack);
// 	free(segments);
// }
//
//
// void print(const char* rawValue, DebugData* data)
// {
// 	printf(rawValue);
// }
//
// void printPop(/*out*/Stack* stack, const char* rawType, DebugData* data);
// {
// 	TkasmType type = getType(rawType);
//
// 	uint8_t* segments = popType(type, stack);
//
// 	bool isSuccess;
// 	void* value = unsegmentType(type, segments, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	printTypeToConsole(type, value);
//
// 	free(segments);
// }
//
// void read(/*out*/Stack* stack, const char* rawType, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
//
// 	bool isSuccess = false;
// 	void* value = readTypeFromConsole(type, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	uint8_t* segments = segmentType(type, value);
// 	pushType(type, segments, stack);
//
// 	free(segments);
// }
//
// void jump(/*out*/uint32_t *index, map_int_t *labelTracker, const char* label)
// {
// 	*index = (int)JUMP_TO_LABEL(labelTracker, label);
// }
//
// bool isEquals0(/*out*/Stack* stack, const char* rawType, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
//
// 	uint8_t* segments = popType(type, /*out*/stack);
// 	pushType(type, segments, /*out*/stack);
//
// 	bool isSuccess;
// 	void* value = unsegmentType(type, segments, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	free(segments);
// 	return isTypeEqual0(type, value);
// }
//
// bool isGreater0(/*out*/Stack* stack, const char* rawType, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
//
// 	uint8_t* segments = popType(type, /*out*/stack);
// 	pushType(type, segments, /*out*/stack);
//
// 	bool isSuccess;
// 	void* value = unsegmentType(type, segments, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	free(segments);
// 	return isTypeGreater0(type, value);
// }
//
// bool isSmaller0(/*out*/Stack* stack, const char* rawType, DebugData* data)
// {
// 	TkasmType type = getType(rawType);
//
// 	uint8_t* segments = popType(type, /*out*/stack);
// 	pushType(type, segments, /*out*/stack);
//
// 	bool isSuccess;
// 	void* value = unsegmentType(type, segments, /*out*/isSuccess);
// 	if (!isSuccess)
// 		exit_TypeIsNotValid(rawType, data);
//
// 	free(segments);
// 	return isTypeSmaller0(type, value);
// }