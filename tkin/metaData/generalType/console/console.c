//
// Created by tim_k on 05/06/2024.
//

#include "console.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../../lib/string/String.h"

#define GET_INPUT(T)		\
({							\
	char line[100];			\
	scanf("%s", line);		\
	T num = (T)atoll(line);	\
	(void*)num;		        \
})

#define GET_INPUT_FLOAT(T)	\
({							\
	char line[100];			\
	scanf("%s", line);		\
	T num = (T)atof(line);	\
	num;					\
})

void* stringToType(const TkasmType *type, const char* rawValue, /*out*/bool *isSuccess)
{
	*isSuccess = true;

	switch (*type)
	{
	case tkasm_char:
		return (void*)rawValue[0];


	case tkasm_uint64:
		return (void*)(uint64_t)atol(rawValue);


	case tkasm_int64:
		return (void*)atol(rawValue);


	case tkasm_uint32:
		return (void*)(uint64_t)atol(rawValue);


	case tkasm_int32:
		return (void*)atol(rawValue);


	case tkasm_uint16:
		return (void*)(uint64_t)atol(rawValue);

	case tkasm_int16:
		return (void*)atol(rawValue);


	case tkasm_uint8:
		return (void*)(uint64_t)atol(rawValue);

	case tkasm_int8:
		return (void*)atol(rawValue);


	case tkasm_float:
	{
		float value = atof(rawValue);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		double value = atof(rawValue);
		return DOUBLE_TO_VOIDP(&value);
	}


	case tkasm_unknown:
	default:
		break;
	}

	*isSuccess = false;
	return nullptr;
}

void* readTypeFromConsole(const TkasmType *type, /*out*/bool *isSuccess)
{
	*isSuccess = true;

	switch (*type)
	{
	case tkasm_char:
		return (void*)GET_INPUT(char);


	case tkasm_uint64:
		return (void*)GET_INPUT(uint64_t);

	case tkasm_int64:
		return (void*)GET_INPUT(int64_t);


	case tkasm_uint32:
		return (void*)GET_INPUT(uint32_t);

	case tkasm_int32:
		return (void*)GET_INPUT(int32_t);


	case tkasm_uint16:
		return (void*)GET_INPUT(uint16_t);

	case tkasm_int16:
		*isSuccess = true;
		return (void*)GET_INPUT(int16_t);


	case tkasm_uint8:
		return (void*)GET_INPUT(uint8_t);

	case tkasm_int8:
		return (void*)GET_INPUT(int8_t);

	case tkasm_float:
	{
		float value = GET_INPUT_FLOAT(float);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		double value = GET_INPUT_FLOAT(double);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}

	*isSuccess = false;
	return nullptr;
}

void printTypeToConsole(const TkasmType *type, void* value)
{
	switch (*type)
	{
	case tkasm_char:
		printf("%c", (char)value);
		break;

	case tkasm_uint64:
		printf("%llu", (uint64_t)value);
		break;

	case tkasm_int64:
		printf("%lld", (int64_t)value);
		break;


	case tkasm_uint32:
		printf("%u", (uint32_t)value);
		break;

	case tkasm_int32:
		printf("%d", (int32_t)value);
		break;


	case tkasm_uint16:
		printf("%u", (uint16_t)value);
		break;

	case tkasm_int16:
		printf("%d", (int16_t)value);
		break;


	case tkasm_uint8:
		printf("%u", (uint16_t)value);
		break;

	case tkasm_int8:
		printf("%d", (int16_t)value);
		break;


	case tkasm_float:
		printf("%f", EXPLICIT_CAST(float, value));
		break;

	case tkasm_double:
		printf("%f", EXPLICIT_CAST(double, value));
		break;


	case tkasm_unknown:
	default:
		printf("null");
		break;
	}
}