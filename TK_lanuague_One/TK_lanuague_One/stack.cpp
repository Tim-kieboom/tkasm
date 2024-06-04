#include "stack.h"
#include "generalTypes.h"

void pushType(TkasmType type, uint8_t* segments, /*out*/Stack* stack)
{
	uint32_t size = abs(getTypeSize(type)) / 8;

	for (int i = size - 1; i >= 0; i--)
	{
		uint8_t segment = segments[i];
		stack->push(segment);
	}
}

uint8_t* popType(TkasmType type, /*out*/Stack* stack)
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