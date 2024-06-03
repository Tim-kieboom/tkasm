#include "generalTypes.h"
#include "stringtools.h"
#include "debug.h"

#define TYPE_IS_BIGGER(type1, type2) (abs(getTypeSize(type1)) > abs(getTypeSize(type2)))

TkasmType getBiggerType(TkasmType type1, TkasmType type2)
{
	if (TYPE_IS_BIGGER(type1, type2))
		return type1;

	return type2;
}


TkasmType getType(string& rawType)
{
	const char* type = rawType.data();
	if (STR_EQUALS(type, "%char"))
	{
		return tkasm_char;
	}
	else if (STR_EQUALS(type, "%uint64"))
	{
		return tkasm_uint64;
	}
	else if (STR_EQUALS(type, "%int64"))
	{
		return tkasm_int64;
	}
	else if (STR_EQUALS(type, "%uint32"))
	{
		return tkasm_uint32;
	}
	else if (STR_EQUALS(type, "%int32"))
	{
		return tkasm_int32;
	}
	else if (STR_EQUALS(type, "%uint16"))
	{
		return tkasm_uint16;
	}
	else if (STR_EQUALS(type, "%int16"))
	{
		return tkasm_int16;
	}
	else if (STR_EQUALS(type, "%uint8"))
	{
		return tkasm_uint8;
	}
	else if (STR_EQUALS(type, "%int8"))
	{
		return tkasm_int8;
	}
	return tkasm_unknown;
}

const char* getTypeString(TkasmType& type)
{
	switch (type)
	{
	case tkasm_char:
		return "char";


	case tkasm_uint64:
		return "uint64";

	case tkasm_int64:
		return "int64";


	case tkasm_uint32:
		return "uint32";

	case tkasm_int32:
		return "int32";


	case tkasm_uint16:
		return "uint16";

	case tkasm_int16:
		return "int16";


	case tkasm_uint8:
		return "uint8";

	case tkasm_int8:
		return "int8";


	case tkasm_unknown:
		return "unknown";

	default:
		break;
	}

	return "null";
}

int16_t getTypeSize(TkasmType& type)
{
	switch (type)
	{
	case tkasm_char:
		return -8;

	case tkasm_uint64:
		return 64;

	case tkasm_int64:
		return -64;


	case tkasm_uint32:
		return 32;

	case tkasm_int32:
		return -32;


	case tkasm_uint16:
		return 16;

	case tkasm_int16:
		return -16;


	case tkasm_uint8:
		return 8;

	case tkasm_int8:
		return -8;

	default:
		return 0;
	}
}

void pushType(TkasmType type, uint8_t* segments, Stack* stack)
{
	uint32_t size = abs(getTypeSize(type)) / 8;

	for (int i = size - 1; i >= 0; i--)
	{
		uint8_t segment = segments[i];
		stack->push(segment);
	}
}

uint8_t* popType(TkasmType type, Stack* stack)
{
	if (stack->empty())
		return nullptr;

	uint32_t size = abs(getTypeSize(type)) / 8;

	uint8_t* segments = new uint8_t[size];
	for (uint8_t i = 0; i < size; i++)
	{
		segments[i] = stack->top();
		stack->pop();
	}

	return segments;
}

template<typename T>
T unsegmentValue(uint8_t* segments)
{
	uint16_t segmentSize = sizeof(T);

	size_t fullValue = 0;
	for (uint8_t i = 0; i < segmentSize;i++)
	{
		fullValue |= (size_t)(segments[i] << (i * 8));
	}

	return EXPLICIT_CAST(T, fullValue);
}

template<typename T>
uint8_t* segmentValue(T value)
{
	uint16_t segmentSize = sizeof(T);
	auto arr = new uint8_t[segmentSize];

	memcpy(arr, &value, segmentSize);
	return arr;
}

uint8_t* segmentType(TkasmType& type, void* value)
{
	switch (type)
	{
	case tkasm_char:
		return segmentValue<char>((char)value);


	case tkasm_uint64:
		return segmentValue<uint64_t>((uint64_t)value);

	case tkasm_int64:
		return segmentValue<int64_t>((int64_t)value);


	case tkasm_uint32:
		return segmentValue<uint32_t>((uint32_t)value);

	case tkasm_int32:
		return segmentValue<int32_t>((int32_t)value);


	case tkasm_uint16:
		return segmentValue<uint16_t>((uint16_t)value);

	case tkasm_int16:
		return segmentValue<int16_t>((int16_t)value);


	case tkasm_uint8:
		return segmentValue<uint8_t>((uint8_t)value);

	case tkasm_int8:
		return segmentValue<int8_t>((int8_t)value);


	case tkasm_unknown:
	default:
		break;
	}

	return nullptr;
}

void* unsegmentType(TkasmType& type, uint8_t* segments, /*out*/bool& isSuccess)
{
	switch (type)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)unsegmentValue<char>(segments);



	case tkasm_uint64:
		isSuccess = true;
		return (void*)unsegmentValue<uint64_t>(segments);


	case tkasm_int64:
		isSuccess = true;
		return (void*)unsegmentValue<int64_t>(segments);



	case tkasm_uint32:
		isSuccess = true;
		return (void*)unsegmentValue<uint32_t>(segments);


	case tkasm_int32:
		isSuccess = true;
		return (void*)unsegmentValue<int32_t>(segments);



	case tkasm_uint16:
		isSuccess = true;
		return (void*)unsegmentValue<uint16_t>(segments);


	case tkasm_int16:
		isSuccess = true;
		return (void*)unsegmentValue<int16_t>(segments);



	case tkasm_uint8:
		isSuccess = true;
		return (void*)unsegmentValue<uint8_t>(segments);


	case tkasm_int8:
		isSuccess = true;
		return (void*)unsegmentValue<int8_t>(segments);



	case tkasm_unknown:
	default:
		break;
	}

	isSuccess = false;
	return nullptr;
}

template<typename T>
T stringTo_uint(string& string)
{
	T prevNumbers = 0;

	int16_t i = -1;
	while (string[++i] != '\0')
	{
		prevNumbers *= 10;
		prevNumbers += (T)(string[i] - '0');
	}

	return prevNumbers;
}

template<typename T>
T stringTo_int(string& string)
{
	T prevNumbers = 0;

	bool isNegative = string[0] == '-';

	int16_t i = (isNegative) ? 0 : -1;
	while (string[++i] != '\0')
	{
		prevNumbers *= 10;
		prevNumbers += (T)(string[i] - '0');
	}

	if (isNegative)
		prevNumbers *= -1;

	return prevNumbers;
}

void* stringToType(TkasmType type, string& rawValue, /*out*/bool& isSuccess)
{
	switch (type)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)rawValue[0];



	case tkasm_uint64:
		isSuccess = true;
		return (void*)stringTo_uint<uint64_t>(rawValue);


	case tkasm_int64:
		isSuccess = true;
		return (void*)stringTo_int<int64_t>(rawValue);



	case tkasm_uint32:
		isSuccess = true;
		return (void*)stringTo_uint<uint32_t>(rawValue);


	case tkasm_int32:
		isSuccess = true;
		return (void*)stringTo_int<int32_t>(rawValue);



	case tkasm_uint16:
		isSuccess = true;
		return (void*)stringTo_uint<uint16_t>(rawValue);


	case tkasm_int16:
		isSuccess = true;
		return (void*)stringTo_int<int16_t>(rawValue);



	case tkasm_uint8:
		isSuccess = true;
		return (void*)stringTo_uint<uint8_t>(rawValue);


	case tkasm_int8:
		isSuccess = true;
		return (void*)stringTo_int<int8_t>(rawValue);



	case tkasm_unknown:
	default:
		break;
	}

	isSuccess = false;
	return nullptr;
}

template<typename T>
T getInput()
{
	T input;
	cin >> input;
	return input;
}

template<typename T>
T getByteInput()
{
	int16_t input;
	cin >> input;
	return (T)input;
}

void* readTypeFromConsole(TkasmType& type, /*out*/bool& isSuccess)
{
	switch (type)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)getInput<char>();


	case tkasm_uint64:
		isSuccess = true;
		return (void*)getInput<uint64_t>();

	case tkasm_int64:
		isSuccess = true;
		return (void*)getInput<int64_t>();


	case tkasm_uint32:
		isSuccess = true;
		return (void*)getInput<uint32_t>();

	case tkasm_int32:
		isSuccess = true;
		return (void*)getInput<int32_t>();


	case tkasm_uint16:
		isSuccess = true;
		return (void*)getInput<uint16_t>();

	case tkasm_int16:
		isSuccess = true;
		return (void*)getInput<int16_t>();


	case tkasm_uint8:
		isSuccess = true;
		return (void*)getByteInput<uint8_t>();

	case tkasm_int8:
		isSuccess = true;
		return (void*)getByteInput<int8_t>();


	case tkasm_unknown:
	default:
		break;
	}

	isSuccess = false;
	return nullptr;
}

void printTypeToConsole(TkasmType& type, void* value)
{
	switch (type)
	{
	case tkasm_char:
		cout << (char)value << endl;
		break;

	case tkasm_uint64:
		cout << (uint64_t)value << endl;
		break;

	case tkasm_int64:
		cout << (int64_t)value << endl;
		break;


	case tkasm_uint32:
		cout << (uint32_t)value << endl;
		break;

	case tkasm_int32:
		cout << (int32_t)value << endl;
		break;

	case tkasm_uint16:
		cout << (uint16_t)value << endl;
		break;

	case tkasm_int16:
		cout << (int16_t)value << endl;
		break;

	case tkasm_uint8:
		cout << (uint16_t)value << endl;
		break;

	case tkasm_int8:
		cout << (int16_t)value << endl;
		break;


	case tkasm_unknown:
	default:
		cout << "null" << endl;
		break;
	}
}

template<typename T>
bool genericEquals(T one, T two)
{
	return one == two;
}

template<typename T>
bool genericGreater(T one, T two)
{
	return one > two;
}

template<typename T>
bool genericSmaller(T one, T two)
{
	return one < two;
}


bool isTypeEqual0(TkasmType& type, void* value)
{
	switch (type)
	{
	case tkasm_char:
		return genericEquals<char>((char)value, (char)0);

	case tkasm_uint64:
		return genericEquals<uint64_t>((uint64_t)value, (uint64_t)0);

	case tkasm_int64:
		return genericEquals<int64_t>((int64_t)value, (int64_t)0);


	case tkasm_uint32:
		return genericEquals<uint32_t>((uint32_t)value, (uint32_t)0);

	case tkasm_int32:
		return genericEquals<int32_t>((int32_t)value, (int32_t)0);


	case tkasm_uint16:
		return genericEquals<uint16_t>((uint16_t)value, (uint16_t)0);

	case tkasm_int16:
		return genericEquals<int16_t>((int16_t)value, (int16_t)0);


	case tkasm_uint8:
		return genericEquals<uint8_t>((uint8_t)value, (uint8_t)0);

	case tkasm_int8:
		return genericEquals<int8_t>((int8_t)value, (int8_t)0);


	case tkasm_unknown:
	default:
		cout << "!<warning> typeEqual0's type unkown!" << endl;
		return false;
	}
}

bool isTypeGreater0(TkasmType& type, void* value)
{
	switch (type)
	{
	case tkasm_char:
		return genericGreater<char>((char)value, (char)0);

	case tkasm_uint64:
		return genericGreater<uint64_t>((uint64_t)value, (uint64_t)0);

	case tkasm_int64:
		return genericGreater<int64_t>((int64_t)value, (int64_t)0);


	case tkasm_uint32:
		return genericGreater<uint32_t>((uint32_t)value, (uint32_t)0);

	case tkasm_int32:
		return genericGreater<int32_t>((int32_t)value, (int32_t)0);


	case tkasm_uint16:
		return genericGreater<uint16_t>((uint16_t)value, (uint16_t)0);

	case tkasm_int16:
		return genericGreater<int16_t>((int16_t)value, (int16_t)0);


	case tkasm_uint8:
		return genericGreater<uint8_t>((uint8_t)value, (uint8_t)0);

	case tkasm_int8:
		return genericGreater<int8_t>((int8_t)value, (int8_t)0);


	case tkasm_unknown:
	default:
		cout << "!<warning> typeGreater0's type unkown!" << endl;
		return false;
	}
}

bool isTypeSmaller0(TkasmType& type, void* value)
{
	switch (type)
	{
	case tkasm_char:
		return genericSmaller<char>((char)value, (char)0);

	case tkasm_uint64:
		return genericSmaller<uint64_t>((uint64_t)value, (uint64_t)0);

	case tkasm_int64:
		return genericSmaller<int64_t>((int64_t)value, (int64_t)0);


	case tkasm_uint32:
		return genericSmaller<uint32_t>((uint32_t)value, (uint32_t)0);

	case tkasm_int32:
		return genericSmaller<int32_t>((int32_t)value, (int32_t)0);


	case tkasm_uint16:
		return genericSmaller<uint16_t>((uint16_t)value, (uint16_t)0);

	case tkasm_int16:
		return genericSmaller<int16_t>((int16_t)value, (int16_t)0);


	case tkasm_uint8:
		return genericSmaller<uint8_t>((uint8_t)value, (uint8_t)0);

	case tkasm_int8:
		return genericSmaller<int8_t>((int8_t)value, (int8_t)0);


	case tkasm_unknown:
	default:
		cout << "!<warning> typeSmaller0's type unkown!" << endl;
		return false;
	}
}