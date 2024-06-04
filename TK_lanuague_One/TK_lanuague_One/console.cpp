#include "console.h"
#include <string>

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


	case tkasm_float:
	{
		isSuccess = true;
		float value = stof(rawValue);
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		isSuccess = true;
		double value = stod(rawValue);
		return DOUBLE_TO_VOIDP(value);
	}

	
	case tkasm_unknown:
	default:
		break;
	}

	isSuccess = false;
	return nullptr;
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

	case tkasm_float:
	{
		isSuccess = true;
		float value = getInput<float>();
		return FLOAT_TO_VOIDP(value);
	}

	case tkasm_double:
	{
		isSuccess = true;
		double value = getInput<double>();
		return DOUBLE_TO_VOIDP(value);
	}

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


	case tkasm_float:
		cout << EXPLICIT_CAST(float, value) << endl;
		break;

	case tkasm_double:
		cout << EXPLICIT_CAST(double, value) << endl;
		break;


	case tkasm_unknown:
	default:
		cout << "null" << endl;
		break;
	}
}