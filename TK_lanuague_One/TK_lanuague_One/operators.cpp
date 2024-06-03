#include "operators.h"
#include "generalTypes.h"

template<typename T>
T genericAdd(void* tValue, void* vValue)
{
	return ((T)tValue + (T)vValue);
}

template<typename T>
T genericSub(void* tValue, void* vValue)
{
	return ((T)tValue - (T)vValue);
}

template<typename T>
T genericMull(void* tValue, void* vValue)
{
	return ((T)tValue * (T)vValue);
}

template<typename T>
T genericDiv(void* tValue, void* vValue)
{
	return ((T)tValue / (T)vValue);
}

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data)
{
	uint8_t* segments1 = popType(type1, stack);
	uint8_t* segments2 = popType(type2, stack);

	if (segments1 == nullptr || segments2 == nullptr)
		exit_stackIsEmpty(data);

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


	case tkasm_unknown:
	default:
		break;
	}
	cout << "!!<error>divTypes type unknown!!" << endl;
	isSuccess = false;
	return nullptr;
}