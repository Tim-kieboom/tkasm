#include "Commands.h"
#include "stringTools.h"

#define JUMP_TO_LABEL(hashMap, strLabel) hashMap[strLabel] 

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

	cout << "!!<error> type not recognized command: " << command << "!!" << endl;
	exit(1);
}
void push(/*out*/Stack* stack, string& rawType, string& rawValue, DebugData* data)
{
	TkasmType type = getType(rawType);

	bool isSuccess;
	void* value = stringToType(type, rawValue, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint8_t* segments = segmentType(type, value);
	pushType(type, segments, stack);

	delete[] segments;
}

void pop(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);
	uint8_t* segments = popType(type, stack);
	delete[] segments;
}


void add(/*out*/Stack* stack, string& rawType1, string rawType2, DebugData* data)
{
	TkasmType type1 = getType(rawType1);
	TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = addTypes(type1, type2, /*out*/stack, /*out*/isSuccess, data);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);

	TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(type, value);

	pushType(type, segments, stack);
	delete[] segments;
}

void sub(/*out*/Stack* stack, string& rawType1, string& rawType2, DebugData* data)
{
	TkasmType type1 = getType(rawType1);
	TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = subTypes(type1, type2, stack, /*out*/isSuccess, data);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);

	TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(type, value);

	pushType(type, segments, stack);
	delete[] segments;
}

void mull(/*out*/Stack* stack, string& rawType1, string& rawType2, DebugData* data)
{
	TkasmType type1 = getType(rawType1);
	TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = mullTypes(type1, type2, stack, /*out*/isSuccess, data);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);

	TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(type, value);

	pushType(type, segments, stack);
	delete[] segments;
}

void div(/*out*/Stack* stack, string& rawType1, string& rawType2, DebugData* data)
{
	TkasmType type1 = getType(rawType1);
	TkasmType type2 = getType(rawType2);

	bool isSuccess;
	void* value = divTypes(type1, type2, stack, /*out*/isSuccess, data);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType1 += string(" and ") += rawType2, data);

	TkasmType type = getBiggerType(type1, type2);
	uint8_t* segments = segmentType(type, value);

	pushType(type, segments, stack);
	delete[] segments;
}


void print(string& rawValue, DebugData* data)
{
	cout << rawValue << endl;
}

void printPop(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);

	uint8_t* segments = popType(type, stack);

	bool isSuccess;
	void* value = unsegmentType(type, segments, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	printTypeToConsole(type, value);

	delete[] segments;
}

void read(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);

	bool isSuccess = false;
	void* value = readTypeFromConsole(type, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	uint8_t* segments = segmentType(type, value);
	pushType(type, segments, stack);

	delete[] segments;
}

void jump(/*out*/uint32_t& index, unordered_map<string, uint32_t>& labelTracker, string& label)
{
	index = JUMP_TO_LABEL(labelTracker, label);
}

bool isEquals0(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);

	uint8_t* segments = popType(type, stack);

	bool isSuccess;
	void* value = unsegmentType(type, segments, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	return isTypeEqual0(type, value);
}

bool isGreater0(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);

	uint8_t* segments = popType(type, stack);

	bool isSuccess;
	void* value = unsegmentType(type, segments, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	return isTypeGreater0(type, value);
}

bool isSmaller0(/*out*/Stack* stack, string& rawType, DebugData* data)
{
	TkasmType type = getType(rawType);

	uint8_t* segments = popType(type, stack);

	bool isSuccess;
	void* value = unsegmentType(type, segments, /*out*/isSuccess);
	if (!isSuccess)
		exit_TypeIsNotValid(rawType, data);

	return isTypeSmaller0(type, value);
}
