//
// Created by tim_k on 06/06/2024.
//

#include "operators.h"
#include <stdio.h>

#define GENERIC_ADD(T, tValue, vValue) (EXPLICIT_CAST(T, tValue) + EXPLICIT_CAST(T, vValue))
#define GENERIC_SUB(T, tValue, vValue) (EXPLICIT_CAST(T, tValue) - EXPLICIT_CAST(T, vValue))
#define GENERIC_MULL(T, tValue, vValue) (EXPLICIT_CAST(T, tValue) * EXPLICIT_CAST(T, vValue))
#define GENERIC_DIV(T, tValue, vValue) (EXPLICIT_CAST(T, tValue) / EXPLICIT_CAST(T, vValue))

#define GENERIC_EQUALS(one, two)  (one == two)
#define GENERIC_GREATER(one, two) (one >  two)
#define GENERIC_SMALLER(one, two) (one <  two)

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data)
{
	const uint8_t* segments1 = popType(&type1, stack);
	const uint8_t* segments2 = popType(&type2, stack);

	if (segments1 == nullptr || segments2 == nullptr)
		exit_stackIsEmpty(data);

	bool isSegmentSuccess1;
	bool isSegmentSuccess2;
	void* value1 = unsegmentType(&type1, segments1, /*out*/&isSegmentSuccess1);
	void* value2 = unsegmentType(&type2, segments2, /*out*/&isSegmentSuccess2);
	if (!isSegmentSuccess1 || !isSegmentSuccess2)
	{
		StringStream *stream = StringStream_new();
		StringStream_appendCharPtr(stream, getTypeString(&type1));
		StringStream_appendCharPtr(stream, " and ");
		StringStream_appendCharPtr(stream, getTypeString(&type2));

		const char* message = StringStream_toCharPtr(stream);
		StringStream_free(stream);
		exit_SegmentationFailed(message, data);
	}

	bool isConvertSucces;
	const TkasmType biggerType = getBiggerType(type1, type2);
	value1 = convertToTypeClass(type1, biggerType, value1, &isConvertSucces);
	value2 = convertToTypeClass(type2, biggerType, value2, &isConvertSucces);

	if (!isConvertSucces)
	{
		const char* typeString = getTypeString(&biggerType);
		exit_CastingFailed(typeString, data);
	}

	switch (biggerType)
	{
	case tkasm_char:
		*isSuccess = true;
		return (void*)GENERIC_ADD(char, value1, value2);


	case tkasm_uint64:
		*isSuccess = true;
		return (void*)GENERIC_ADD(uint64_t, value1, value2);


	case tkasm_int64:
		*isSuccess = true;
		return (void*)GENERIC_ADD(int64_t, value1, value2);



	case tkasm_uint32:
		*isSuccess = true;
		return (void*)GENERIC_ADD(uint32_t, value1, value2);


	case tkasm_int32:
		*isSuccess = true;
		return (void*)GENERIC_ADD(int32_t, value1, value2);



	case tkasm_uint16:
		*isSuccess = true;
		return (void*)GENERIC_ADD(uint16_t, value1, value2);


	case tkasm_int16:
		*isSuccess = true;
		return (void*)GENERIC_ADD(int16_t, value1, value2);



	case tkasm_uint8:
		*isSuccess = true;
		return (void*)GENERIC_ADD(uint8_t, value1, value2);


	case tkasm_int8:
		*isSuccess = true;
		return (void*)GENERIC_ADD(int8_t, value1, value2);

	case tkasm_float:
	{
		*isSuccess = true;
		float value = GENERIC_ADD(float, value1, value2);
		return FLOAT_TO_VOIDP(&value);
	}


	case tkasm_double:
	{
		*isSuccess = true;
		double value = GENERIC_ADD(double, value1, value2);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	printf("!!<error>subTypes type unknown!!");
	*isSuccess = false;
	return nullptr;
}

void* subTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data)
{
	const uint8_t* segments1 = popType(&type1, stack);
	const uint8_t* segments2 = popType(&type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(&type1, segments1, /*out*/&isSuccess1);
	void* value1 = unsegmentType(&type2, segments2, /*out*/&isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		StringStream *stream = StringStream_new();
		StringStream_appendCharPtr(stream, getTypeString(&type1));
		StringStream_appendCharPtr(stream, " and ");
		StringStream_appendCharPtr(stream, getTypeString(&type2));

		const char* message = StringStream_toCharPtr(stream);
		StringStream_free(stream);
		exit_SegmentationFailed(message, data);
	}

	bool isConvertSucces;
	const TkasmType biggerType = getBiggerType(type1, type2);
	value1 = convertToTypeClass(type1, biggerType, value1, &isConvertSucces);
	value2 = convertToTypeClass(type2, biggerType, value2, &isConvertSucces);

	if (!isConvertSucces)
	{
		const char* typeString = getTypeString(&biggerType);
		exit_CastingFailed(typeString, data);
	}

	switch (biggerType)
	{
	case tkasm_char:
		*isSuccess = true;
		return (void*)GENERIC_SUB(char, value1, value2);


	case tkasm_uint64:
		*isSuccess = true;
		return (void*)GENERIC_SUB(uint64_t, value1, value2);


	case tkasm_int64:
		*isSuccess = true;
		return (void*)GENERIC_SUB(int64_t, value1, value2);



	case tkasm_uint32:
		*isSuccess = true;
		return (void*)GENERIC_SUB(uint32_t, value1, value2);


	case tkasm_int32:
		*isSuccess = true;
		return (void*)GENERIC_SUB(int32_t, value1, value2);


	case tkasm_uint16:
		*isSuccess = true;
		return (void*)GENERIC_SUB(uint16_t, value1, value2);


	case tkasm_int16:
		*isSuccess = true;
		return (void*)GENERIC_SUB(int16_t, value1, value2);



	case tkasm_uint8:
		*isSuccess = true;
		return (void*)GENERIC_SUB(uint8_t, value1, value2);


	case tkasm_int8:
		*isSuccess = true;
		return (void*)GENERIC_SUB(int8_t, value1, value2);

	case tkasm_float:
	{
		*isSuccess = true;
		float value = GENERIC_SUB(float, value1, value2);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		*isSuccess = true;
		double value = GENERIC_SUB(double, value1, value2);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	printf("!!<error>subTypes type unknown!!");
	*isSuccess = false;
	return nullptr;
}

void* mullTypes(const TkasmType type1, const TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data)
{
	const uint8_t* segments1 = popType(&type1, stack);
	const uint8_t* segments2 = popType(&type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(&type1, segments1, /*out*/&isSuccess1);
	void* value1 = unsegmentType(&type2, segments2, /*out*/&isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		StringStream *stream = StringStream_new();
		StringStream_appendCharPtr(stream, getTypeString(&type1));
		StringStream_appendCharPtr(stream, " and ");
		StringStream_appendCharPtr(stream, getTypeString(&type2));

		const char* message = StringStream_toCharPtr(stream);
		StringStream_free(stream);
		exit_SegmentationFailed(message, data);
	}

	bool isConvertSucces;
	const TkasmType biggerType = getBiggerType(type1, type2);
	value1 = convertToTypeClass(type1, biggerType, value1, &isConvertSucces);
	value2 = convertToTypeClass(type2, biggerType, value2, &isConvertSucces);

	if (!isConvertSucces)
	{
		const char* typeString = getTypeString(&biggerType);
		exit_CastingFailed(typeString, data);
	}

	switch (biggerType)
	{
	case tkasm_char:
		*isSuccess = true;
		return (void*)GENERIC_MULL(char, value1, value2);


	case tkasm_uint64:
		*isSuccess = true;
		return (void*)GENERIC_MULL(uint64_t, value1, value2);


	case tkasm_int64:
		*isSuccess = true;
		return (void*)GENERIC_MULL(int64_t, value1, value2);


	case tkasm_uint32:
		*isSuccess = true;
		return (void*)GENERIC_MULL(uint32_t, value1, value2);


	case tkasm_int32:
		*isSuccess = true;
		return (void*)GENERIC_MULL(int32_t, value1, value2);


	case tkasm_uint16:
		*isSuccess = true;
		return (void*)GENERIC_MULL(uint16_t, value1, value2);


	case tkasm_int16:
		*isSuccess = true;
		return (void*)GENERIC_MULL(int16_t, value1, value2);



	case tkasm_uint8:
		*isSuccess = true;
		return (void*)GENERIC_MULL(uint8_t, value1, value2);


	case tkasm_int8:
		*isSuccess = true;
		return (void*)GENERIC_MULL(int8_t, value1, value2);

	case tkasm_float:
	{
		*isSuccess = true;
		float value = GENERIC_MULL(float, value1, value2);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		*isSuccess = true;
		double value = GENERIC_MULL(double, value1, value2);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	printf("!!<error>mullTypes type unknown!!");
	*isSuccess = false;
	return nullptr;
}

void* divTypes(const TkasmType type1, const TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data)
{
	const uint8_t* segments1 = popType(&type1, stack);
	const uint8_t* segments2 = popType(&type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(&type1, segments1, /*out*/&isSuccess1);
	void* value1 = unsegmentType(&type2, segments2, /*out*/&isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		StringStream *stream = StringStream_new();
		StringStream_appendCharPtr(stream, getTypeString(&type1));
		StringStream_appendCharPtr(stream, " and ");
		StringStream_appendCharPtr(stream, getTypeString(&type2));

		const char* message = StringStream_toCharPtr(stream);
		StringStream_free(stream);
		exit_SegmentationFailed(message, data);
	}

	bool isConvertSucces;
	const TkasmType biggerType = getBiggerType(type1, type2);
	value1 = convertToTypeClass(type1, biggerType, value1, &isConvertSucces);
	value2 = convertToTypeClass(type2, biggerType, value2, &isConvertSucces);

	if (!isConvertSucces)
	{
		const char* typeString = getTypeString(&biggerType);
		exit_CastingFailed(typeString, data);
	}

	switch (biggerType)
	{
	case tkasm_char:
		*isSuccess = true;
		return (void*)GENERIC_DIV(char, value1, value2);


	case tkasm_uint64:
		*isSuccess = true;
		return (void*)GENERIC_DIV(uint64_t, value1, value2);


	case tkasm_int64:
		*isSuccess = true;
		return (void*)GENERIC_DIV(int64_t, value1, value2);


	case tkasm_uint32:
		*isSuccess = true;
		return (void*)GENERIC_DIV(uint32_t, value1, value2);


	case tkasm_int32:
		*isSuccess = true;
		return (void*)GENERIC_DIV(int32_t, value1, value2);


	case tkasm_uint16:
		*isSuccess = true;
		return (void*)GENERIC_DIV(uint16_t, value1, value2);


	case tkasm_int16:
		*isSuccess = true;
		return (void*)GENERIC_DIV(int16_t, value1, value2);


	case tkasm_uint8:
		*isSuccess = true;
		return (void*)GENERIC_DIV(uint8_t, value1, value2);


	case tkasm_int8:
		*isSuccess = true;
		return (void*)GENERIC_DIV(int8_t, value1, value2);

	case tkasm_float:
	{
		*isSuccess = true;
		const float value = GENERIC_DIV(float, value1, value2);
		return FLOAT_TO_VOIDP(&value);
	}

	case tkasm_double:
	{
		*isSuccess = true;
		const double value = GENERIC_DIV(double, value1, value2);
		return DOUBLE_TO_VOIDP(&value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	printf("!!<error>divTypes type unknown!!");
	*isSuccess = false;
	return nullptr;
}


bool isTypeEqual0(const TkasmType *type, void* value)
{
	switch (*type)
	{
	case tkasm_char:
		return (void*)GENERIC_EQUALS((char)value, (char)0);

	case tkasm_uint64:
		return (void*)GENERIC_EQUALS(*(uint64_t*)&value, (uint64_t)0);

	case tkasm_int64:
		return (void*)GENERIC_EQUALS(*(int64_t*)&value, (int64_t)0);


	case tkasm_uint32:
		return (void*)GENERIC_EQUALS(*(uint32_t*)&value, (uint32_t)0);

	case tkasm_int32:
		return (void*)GENERIC_EQUALS(*(int32_t*)&value, (int32_t)0);


	case tkasm_uint16:
		return (void*)GENERIC_EQUALS(*(uint16_t*)&value, (uint16_t)0);

	case tkasm_int16:
		return (void*)GENERIC_EQUALS(*(int16_t*)&value, (int16_t)0);


	case tkasm_uint8:
		return (void*)GENERIC_EQUALS(*(uint8_t*)&value, (uint8_t)0);

	case tkasm_int8:
		return (void*)GENERIC_EQUALS(*(int8_t*)&value, (int8_t)0);


	case tkasm_float:
		return (void*)GENERIC_EQUALS(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return (void*)GENERIC_EQUALS(EXPLICIT_CAST(double, value), (double)0);


	case tkasm_unknown:
	default:
		printf("!<warning> typeEqual0's type unkown!");
		return false;
	}
}

bool isTypeGreater0(const TkasmType *type, void* value)
{
	switch (*type)
	{
	case tkasm_char:
		return (void*)GENERIC_GREATER(*(char*)&value, (char)0);

	case tkasm_uint64:
		return (void*)GENERIC_GREATER(*(uint64_t*)&value, (uint64_t)0);

	case tkasm_int64:
		return (void*)GENERIC_GREATER(*(int64_t*)&value, (int64_t)0);


	case tkasm_uint32:
		return (void*)GENERIC_GREATER(*(uint32_t*)&value, (uint32_t)0);

	case tkasm_int32:
		return (void*)GENERIC_GREATER(*(int32_t*)&value, (int32_t)0);


	case tkasm_uint16:
		return (void*)GENERIC_GREATER(*(uint16_t*)&value, (uint16_t)0);

	case tkasm_int16:
		return (void*)GENERIC_GREATER(*(int16_t*)&value, (int16_t)0);


	case tkasm_uint8:
		return (void*)GENERIC_GREATER(*(uint8_t*)&value, (uint8_t)0);

	case tkasm_int8:
		return (void*)GENERIC_GREATER(*(int8_t*)&value, (int8_t)0);


	case tkasm_float:
		return (void*)GENERIC_GREATER(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return (void*)GENERIC_GREATER(EXPLICIT_CAST(double, value), (double)0);


	case tkasm_unknown:
	default:
		printf("!<warning> typeGreater0's type unkown!");
		return false;
	}
}

bool isTypeSmaller0(const TkasmType *type, void* value)
{
	switch (*type)
	{
	case tkasm_char:
		return (void*)GENERIC_SMALLER(*(char*)&value, (char)0);

	case tkasm_uint64:
		return (void*)GENERIC_SMALLER(*(uint64_t*)&value, (uint64_t)0);

	case tkasm_int64:
		return (void*)GENERIC_SMALLER(*(int64_t*)&value, (int64_t)0);


	case tkasm_uint32:
		return (void*)GENERIC_SMALLER(*(uint32_t*)&value, (uint32_t)0);

	case tkasm_int32:
		return (void*)GENERIC_SMALLER(*(int32_t*)&value, (int32_t)0);


	case tkasm_uint16:
		return (void*)GENERIC_SMALLER(*(uint16_t*)&value, (uint16_t)0);

	case tkasm_int16:
		return (void*)GENERIC_SMALLER(*(int16_t*)&value, (int16_t)0);


	case tkasm_uint8:
		return (void*)GENERIC_SMALLER(*(uint8_t*)&value, (uint8_t)0);

	case tkasm_int8:
		return (void*)GENERIC_SMALLER(*(int8_t*)&value, (int8_t)0);


	case tkasm_float:
		return (void*)GENERIC_SMALLER(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return (void*)GENERIC_SMALLER(EXPLICIT_CAST(double, value), (double)0);


	case tkasm_unknown:
	default:
		printf("!<warning> typeSmaller0's type unkown!");
		return false;
	}
}