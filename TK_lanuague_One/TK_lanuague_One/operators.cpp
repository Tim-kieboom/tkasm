#include "operators.h"
#include "generalTypes.h"

template<typename T>
T genericAdd(void* tValue, void* vValue)
{
	return (EXPLICIT_CAST(T, tValue) + EXPLICIT_CAST(T, vValue));
}

template<typename T>
T genericSub(void* tValue, void* vValue)
{
	return (EXPLICIT_CAST(T, tValue) - EXPLICIT_CAST(T, vValue));
}

template<typename T>
T genericMull(void* tValue, void* vValue)
{
	return (EXPLICIT_CAST(T, tValue) * EXPLICIT_CAST(T, vValue));
}

template<typename T>
T genericDiv(void* tValue, void* vValue)
{
	return (EXPLICIT_CAST(T, tValue) / EXPLICIT_CAST(T, vValue));
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

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data)
{
	uint8_t* segments1 = popType(type1, stack);
	uint8_t* segments2 = popType(type2, stack);

	if (segments1 == nullptr || segments2 == nullptr)
		exit_stackIsEmpty(data);

	bool isSegmentSuccess1;
	bool isSegmentSuccess2;
	void* value1 = unsegmentType(type1, segments1, /*out*/isSegmentSuccess1);
	void* value2 = unsegmentType(type2, segments2, /*out*/isSegmentSuccess2);
	if (!isSegmentSuccess1 || !isSegmentSuccess2)
	{
		string message = string(getTypeString(type1)) += string(" and ") += string(getTypeString(type2));
		exit_SegmentationFailed(message, data);
	}

	bool isConvertSucces;
	TkasmType biggerType = getBiggerType(type1, type2);
	if (type2 == biggerType)
		value1 = convertToTypeClass(type1, biggerType, value1, isConvertSucces);
	else
		value2 = convertToTypeClass(type2, biggerType, value2, isConvertSucces);

	if (!isConvertSucces)
	{
		string typeString = getTypeString(biggerType);
		exit_CastingFailed(typeString, data);
	}

	switch (biggerType)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)genericAdd<char>(value1, value2);


	case tkasm_uint64:
		isSuccess = true;
		return (void*)genericAdd<uint64_t>(value1, value2);


	case tkasm_int64:
		isSuccess = true;
		return (void*)genericAdd<int64_t>(value1, value2);



	case tkasm_uint32:
		isSuccess = true;
		return (void*)genericAdd<uint32_t>(value1, value2);


	case tkasm_int32:
		isSuccess = true;
		return (void*)genericAdd<int32_t>(value1, value2);



	case tkasm_uint16:
		isSuccess = true;
		return (void*)genericAdd<uint16_t>(value1, value2);


	case tkasm_int16:
		isSuccess = true;
		return (void*)genericAdd<int16_t>(value1, value2);



	case tkasm_uint8:
		isSuccess = true;
		return (void*)genericAdd<uint8_t>(value1, value2);


	case tkasm_int8:
		isSuccess = true;
		return (void*)genericAdd<int8_t>(value1, value2);

	case tkasm_float:
	{
		isSuccess = true;
		float value = genericAdd<float>(value1, value2);
		return FLOAT_TO_VOIDP(value);
	}


	case tkasm_double:
	{
		isSuccess = true;
		double value = genericAdd<double>(value1, value2);
		return DOUBLE_TO_VOIDP(value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	cout << "!!<error>subTypes type unknown!!" << endl;
	isSuccess = false;
	return nullptr;
}

void* subTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data)
{
	uint8_t* segments1 = popType(type1, stack);
	uint8_t* segments2 = popType(type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(type1, segments1, /*out*/isSuccess1);
	void* value1 = unsegmentType(type2, segments2, /*out*/isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		string message = string(getTypeString(type1)) += string(" and ") += string(getTypeString(type2));
		exit_SegmentationFailed(message, data);
	}

	TkasmType biggerType = getBiggerType(type1, type2);

	switch (biggerType)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)genericSub<char>(value1, value2);


	case tkasm_uint64:
		isSuccess = true;
		return (void*)genericSub<uint64_t>(value1, value2);


	case tkasm_int64:
		isSuccess = true;
		return (void*)genericSub<int64_t>(value1, value2);



	case tkasm_uint32:
		isSuccess = true;
		return (void*)genericSub<uint32_t>(value1, value2);


	case tkasm_int32:
		isSuccess = true;
		return (void*)genericSub<int32_t>(value1, value2);


	case tkasm_uint16:
		isSuccess = true;
		return (void*)genericSub<uint16_t>(value1, value2);


	case tkasm_int16:
		isSuccess = true;
		return (void*)genericSub<int16_t>(value1, value2);



	case tkasm_uint8:
		isSuccess = true;
		return (void*)genericSub<uint8_t>(value1, value2);


	case tkasm_int8:
		isSuccess = true;
		return (void*)genericSub<int8_t>(value1, value2);

	case tkasm_float:
	{
		isSuccess = true;
		float value = genericSub<float>(value1, value2);
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		isSuccess = true;
		double value = genericSub<double>(value1, value2);
		return DOUBLE_TO_VOIDP(value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	cout << "!!<error>subTypes type unknown!!" << endl;
	isSuccess = false;
	return nullptr;
}

void* mullTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data)
{
	uint8_t* segments1 = popType(type1, stack);
	uint8_t* segments2 = popType(type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(type1, segments1, /*out*/isSuccess1);
	void* value1 = unsegmentType(type2, segments2, /*out*/isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		string message = string(getTypeString(type1)) += string(" and ") += string(getTypeString(type2));
		exit_SegmentationFailed(message, data);
	}

	TkasmType biggerType = getBiggerType(type1, type2);

	switch (biggerType)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)genericMull<char>(value1, value2);


	case tkasm_uint64:
		isSuccess = true;
		return (void*)genericMull<uint64_t>(value1, value2);


	case tkasm_int64:
		isSuccess = true;
		return (void*)genericMull<int64_t>(value1, value2);


	case tkasm_uint32:
		isSuccess = true;
		return (void*)genericMull<uint32_t>(value1, value2);


	case tkasm_int32:
		isSuccess = true;
		return (void*)genericMull<int32_t>(value1, value2);


	case tkasm_uint16:
		isSuccess = true;
		return (void*)genericMull<uint16_t>(value1, value2);


	case tkasm_int16:
		isSuccess = true;
		return (void*)genericMull<int16_t>(value1, value2);



	case tkasm_uint8:
		isSuccess = true;
		return (void*)genericMull<uint8_t>(value1, value2);


	case tkasm_int8:
		isSuccess = true;
		return (void*)genericMull<int8_t>(value1, value2);

	case tkasm_float:
	{
		isSuccess = true;
		float value = genericMull<float>(value1, value2);
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		isSuccess = true;
		double value = genericMull<double>(value1, value2);
		return DOUBLE_TO_VOIDP(value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	cout << "!!<error>mullTypes type unknown!!" << endl;
	isSuccess = false;
	return nullptr;
}

void* divTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data)
{
	uint8_t* segments1 = popType(type1, stack);
	uint8_t* segments2 = popType(type2, stack);

	bool isSuccess1;
	bool isSuccess2;
	void* value2 = unsegmentType(type1, segments1, /*out*/isSuccess1);
	void* value1 = unsegmentType(type2, segments2, /*out*/isSuccess2);
	if (!isSuccess1 || !isSuccess2)
	{
		string message = string(getTypeString(type1)) += string(" and ") += string(getTypeString(type2));
		exit_SegmentationFailed(message, data);
	}

	TkasmType biggerType = getBiggerType(type1, type2);

	switch (biggerType)
	{
	case tkasm_char:
		isSuccess = true;
		return (void*)genericDiv<char>(value1, value2);


	case tkasm_uint64:
		isSuccess = true;
		return (void*)genericDiv<uint64_t>(value1, value2);


	case tkasm_int64:
		isSuccess = true;
		return (void*)genericDiv<int64_t>(value1, value2);


	case tkasm_uint32:
		isSuccess = true;
		return (void*)genericDiv<uint32_t>(value1, value2);


	case tkasm_int32:
		isSuccess = true;
		return (void*)genericDiv<int32_t>(value1, value2);


	case tkasm_uint16:
		isSuccess = true;
		return (void*)genericDiv<uint16_t>(value1, value2);


	case tkasm_int16:
		isSuccess = true;
		return (void*)genericDiv<int16_t>(value1, value2);


	case tkasm_uint8:
		isSuccess = true;
		return (void*)genericDiv<uint8_t>(value1, value2);


	case tkasm_int8:
		isSuccess = true;
		return (void*)genericDiv<int8_t>(value1, value2);

	case tkasm_float:
	{
		isSuccess = true;
		float value = genericDiv<float>(value1, value2);
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		isSuccess = true;
		double value = genericDiv<double>(value1, value2);
		return DOUBLE_TO_VOIDP(value);
	}

	case tkasm_unknown:
	default:
		break;
	}
	cout << "!!<error>divTypes type unknown!!" << endl;
	isSuccess = false;
	return nullptr;
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


	case tkasm_float:
		return genericEquals<float>(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return genericEquals<int8_t>(EXPLICIT_CAST(double, value), (double)0);


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


	case tkasm_float:
		return genericGreater<float>(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return genericGreater<int8_t>(EXPLICIT_CAST(double, value), (double)0);


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


	case tkasm_float:
		return genericSmaller<float>(EXPLICIT_CAST(float, value), (float)0);

	case tkasm_double:
		return genericSmaller<int8_t>(EXPLICIT_CAST(double, value), (double)0);


	case tkasm_unknown:
	default:
		cout << "!<warning> typeSmaller0's type unkown!" << endl;
		return false;
	}
}