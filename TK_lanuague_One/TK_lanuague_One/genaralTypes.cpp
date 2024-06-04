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