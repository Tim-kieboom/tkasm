//
// Created by tim_k on 06/06/2024.
//

#include "stackMemory.h"
#include <stdlib.h>
#include <string.h>

#define SEGMENT_VALUE(T, value)										\
({																	\
	uint16_t segmentSize = sizeof(T);								\
																	\
	uint8_t* arr = (uint8_t*)malloc(segmentSize * sizeof(uint8_t));	\
																	\
	T castValue	= (T)value;											\
	memcpy(arr, &castValue, segmentSize);							\
	(void*)arr;														\
})

#define UNSEGMENT_VALUE(T, segments)								\
({																	\
	uint16_t segmentSize = sizeof(T);								\
																	\
	uint64_t fullValue = 0;											\
	for (uint8_t i = 0; i < segmentSize;i++)						\
		fullValue |= (uint64_t)(segments[i] << (i * 8));			\
																	\
    EXPLICIT_CAST(T, fullValue);									\
})

void pushType(const TkasmType *type, const uint8_t* segments, /*out*/Stack* stack)
{
	const uint32_t size = abs(getTypeSize(type)) / 8;

	for (int i = size - 1; i >= 0; i--)
	{
		const uint8_t segment = segments[i];
		Stack_push(stack, segment);
	}
}

uint8_t* popType(const TkasmType *type, /*out*/Stack* stack)
{
	if (stack->top == -1)
		return nullptr;

	const uint32_t size = abs(getTypeSize(type)) / 8;
	uint8_t* segments = malloc(size * sizeof(uint8_t));

	for (uint8_t i = 0; i < size; i++)
		segments[i] = Stack_pop(stack);

	return segments;
}

uint8_t* peekType(const TkasmType *type, const Stack* stack)
{
	if (stack->top == -1)
		return nullptr;

	const uint32_t size = abs(getTypeSize(type)) / 8;
	uint8_t* segments = malloc(size * sizeof(uint8_t));

	for (uint8_t i = 0; i < size; i++)
		segments[i] = Stack_peekAt(stack, i);

	return segments;
}

uint8_t* peekArrayIndexType(const TkasmType *type, const Stack* stack, const uint32_t index)
{
	if (stack->top == -1)
		return nullptr;

	const uint32_t size = abs(getTypeSize(type)) / 8;
	uint8_t* segments = malloc(size * sizeof(uint8_t));

	const size_t indexOffset = index*size;
	for (uint8_t i = 0; i < size; i++)
		segments[i] = Stack_peekAt(stack, i+indexOffset);

	return segments;
}

uint8_t* segmentType(const TkasmType *type, void* value)
{
	switch (*type)
	{
	case tkasm_char:
		return (uint8_t*)SEGMENT_VALUE(char, (char)value);


	case tkasm_uint64:
		return (uint8_t*)SEGMENT_VALUE(uint64_t, (uint64_t)value);

	case tkasm_int64:
		return (uint8_t*)SEGMENT_VALUE(int64_t, (int64_t)value);


	case tkasm_uint32:
		return (uint8_t*)SEGMENT_VALUE(uint32_t, (uint32_t)value);

	case tkasm_int32:
		return (uint8_t*)SEGMENT_VALUE(int32_t, (int32_t)value);


	case tkasm_uint16:
		return (uint8_t*)SEGMENT_VALUE(uint16_t, (uint16_t)value);

	case tkasm_int16:
		return (uint8_t*)SEGMENT_VALUE(int16_t, (int16_t)value);


	case tkasm_uint8:
		return (uint8_t*)SEGMENT_VALUE(uint8_t, (uint8_t)value);

	case tkasm_int8:
		return (uint8_t*)SEGMENT_VALUE(int8_t, (int8_t)value);

	case tkasm_float:
		return (uint8_t*)SEGMENT_VALUE(float, *(float*)&value);

	case tkasm_double:
		return (uint8_t*)SEGMENT_VALUE(double, *(double*)&value);


	case tkasm_unknown:
	default:
		break;
	}

	return nullptr;
}

void* unsegmentType(const TkasmType *type, const uint8_t* segments, /*out*/bool *isSuccess)
{
	*isSuccess = true;
	switch (*type)
	{
	case tkasm_char:
		return (void*)UNSEGMENT_VALUE(char, segments);

	case tkasm_uint64:
		return (void*)UNSEGMENT_VALUE(uint64_t, segments);

	case tkasm_int64:
		return (void*)UNSEGMENT_VALUE(int64_t, segments);


	case tkasm_uint32:
		return (void*)UNSEGMENT_VALUE(uint32_t, segments);

	case tkasm_int32:
		return (void*)UNSEGMENT_VALUE(int32_t, segments);


	case tkasm_uint16:
		return (void*)UNSEGMENT_VALUE(uint16_t, segments);

	case tkasm_int16:
		return (void*)UNSEGMENT_VALUE(int16_t, segments);


	case tkasm_uint8:
		return (void*)UNSEGMENT_VALUE(uint8_t, segments);

	case tkasm_int8:
		return (void*)UNSEGMENT_VALUE(int8_t, segments);


	case tkasm_float:
	{
		const float value = UNSEGMENT_VALUE(float, segments);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		const double value = UNSEGMENT_VALUE(double, segments);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}

	*isSuccess = false;
	return nullptr;
}