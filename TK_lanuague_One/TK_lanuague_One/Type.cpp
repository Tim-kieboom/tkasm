#include "Types.h"
#include "stringtools.h"

TkasmType getType(string rawType)
{
	const char* type = rawType.data();
	if (STR_EQUALS(type, "%char"))
	{
		return tkasm_char;
	}
	else if (STR_EQUALS(type, "%uint32"))
	{
		return tkasm_uint32;
	}

	return tkasm_unknown;
}

const char* getTypeString(TkasmType type)
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
}

int16_t getTypeSize(TkasmType type)
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
		break;
	}
}