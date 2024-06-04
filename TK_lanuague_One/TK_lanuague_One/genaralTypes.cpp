#include "Types.h"
#include "stringtools.h"
#include "debug.h"

#define TYPE_IS_BIGGER(type1, type2) (abs(getTypeSize(type1)) > abs(getTypeSize(type2)))
#define TO_TYPECLASS_FLOAT(type, newValue)  \
											\
	if (type == tkasm_float)				\
	{										\
		float flValue = (float)newValue;	\
		return FLOAT_TO_VOIDP(flValue);		\
	}										\
	else if (type == tkasm_double)			\
	{										\
		double dbValue = (double)newValue;	\
		return DOUBLE_TO_VOIDP(dbValue);	\
	}	

TkasmType getBiggerType(TkasmType type1, TkasmType type2)
{
	TkasmTypeClass typeClass1 = getTypeClass(type1);
	TkasmTypeClass typeClass2 = getTypeClass(type2);

	if (typeClass1 > typeClass2)
	{
		return type1;
	}
	else if (typeClass1 < typeClass2)
	{
		return type2;
	}

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

	else if (STR_EQUALS(type, "%float"))
	{
		return tkasm_float;
	}
	else if (STR_EQUALS(type, "%double"))
	{
		return tkasm_double;
	}
	return tkasm_unknown;
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

	case tkasm_float:
		return "float";

	case tkasm_double:
		return "double";

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


	case tkasm_float:
		return -32;

	case tkasm_double:
		return -64;

	default:
		return 0;
	}
}

TkasmTypeClass getTypeClass(TkasmType& type)
{
	switch (type)
	{
	case tkasm_uint64:
	case tkasm_uint32:
	case tkasm_uint16:
	case tkasm_uint8:
		return tkasmClass_uint;


	case tkasm_char:
	case tkasm_int64:
	case tkasm_int32:
	case tkasm_int16:
	case tkasm_int8:
		return tkasmClass_int;

	case tkasm_float:
	case tkasm_double:
		return tkasmClass_float;

	case tkasm_unknown:
	default:
		return tkasmClass_unknown;
	}
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

	case tkasm_float:
		return segmentValue<float>(*(float*)&value);

	case tkasm_double:
		return segmentValue<double>(*(double*)&value);


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


	case tkasm_float:
	{
		float value = unsegmentValue<float>(segments);
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		double value = unsegmentValue<double>(segments);
		return DOUBLE_TO_VOIDP(value);
	}

	case tkasm_unknown:
	default:
		break;
	}

	isSuccess = false;
	return nullptr;
}

void* convertToTypeClass(TkasmType& oldType, TkasmType& newType, void* value, /*out*/bool& isSuccess)
{
	TkasmTypeClass oldClass = getTypeClass(oldType);
	TkasmTypeClass newClass = getTypeClass(newType);
	isSuccess = true;

	switch (oldClass)
	{
	case tkasmClass_int:
	{
		int64_t newValue = (int64_t)value;
		switch (newClass)
		{
		case tkasmClass_int:
			return (void*)(int64_t)newValue;

		case tkasmClass_uint:
			return (void*)((uint64_t)newValue);

		case tkasmClass_float:
		{
			TO_TYPECLASS_FLOAT(newType, newValue);
		}

		default:
			break;
		}
	}
	case tkasmClass_uint:
	{
		uint64_t newValue = (uint64_t)value;
		switch (newClass)
		{
		case tkasmClass_int:
			return (void*)(int64_t)newValue;

		case tkasmClass_uint:
			return (void*)((uint64_t)newValue);

		case tkasmClass_float:
		{
			TO_TYPECLASS_FLOAT(newType, newValue);
		}
		default:
			break;
		}
	}
	case tkasmClass_float:
	{
		double newValue = *(double*)&value;
		switch (newClass)
		{
		case tkasmClass_int:
			return (void*)(int64_t)newValue;

		case tkasmClass_uint:
			return (void*)((uint64_t)newValue);

		case tkasmClass_float:
		{
			TO_TYPECLASS_FLOAT(newType, newValue);
		}
		default:
			break;
		}
	}
	default:
		break;
	}

	isSuccess = false;
	cout << "!!<warning> convertToTypeClass did nothing!!" << endl;
	return value;
}