//
// Created by tim_k on 05/06/2024.
//

#include "generalType.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../lib/stringTools/stringTools.h"
#define TYPE_IS_BIGGER(type1, type2) (abs(getTypeSize(type1)) > abs(getTypeSize(type2)))

#define TO_TYPECLASS_FLOAT(type, value)             \
({                                                  \
    void* retValue;                                 \
    if (type == tkasm_float)				        \
    {										        \
        float flValue = (float)value;	            \
        return FLOAT_TO_VOIDP(&flValue);            \
    }										        \
    else if (type == tkasm_double)			        \
    {										        \
        double dbValue = (double)value;	            \
        return DOUBLE_TO_VOIDP(&dbValue);	        \
    }                                               \
    retValue;                                       \
})

#define RECAST_INT(type, value)                     \
switch(type)                                        \
{                                                   \
    case tkasm_char:                                \
        return (void*)(char)value;                  \
                                                    \
    case tkasm_int64:                               \
        return (void*)(int64_t)value;               \
                                                    \
    case tkasm_int32:                               \
        return (void*)(int32_t)value;               \
                                                    \
    case tkasm_int16:                               \
        return (void*)(int16_t)value;               \
                                                    \
    case tkasm_int8:                                \
        return (void*)(int8_t)value;                \
}

#define RECAST_VALUE(class, type, value)            \
({                                                  \
     switch (class)                                 \
     {                                              \
        case tkasmClass_int:                        \
            RECAST_INT(type, value);                \
                                                    \
        case tkasmClass_uint:                       \
            return (void*)((uint64_t)value);        \
                                                    \
        case tkasmClass_float:                      \
            TO_TYPECLASS_FLOAT(type, value);        \
                                                    \
        default:                                    \
            break;                                  \
    }                                               \
    *isSuccess = false;                             \
    return NULL;                                    \
})

TkasmTypeClass getTypeClass_FromStr(const char *rawType)
{
    const TkasmType type = getType(rawType);
    return getTypeClass(&type);
}

TkasmTypeClass getTypeClass(const TkasmType *type)
{
    switch (*type)
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

        case tkasm_arrayChar:
        case tkasm_arrayUint64:
        case tkasm_arrayInt64:
        case tkasm_arrayUint32:
        case tkasm_arrayInt32:
        case tkasm_arrayUint16:
        case tkasm_arrayInt16:
        case tkasm_arrayUint8:
        case tkasm_arrayInt8:
        case tkasm_arrayFloat:
        case tkasm_arrayDouble:
            return tkasmClass_array;

        case tkasm_returnPointer:
                    return tkasmClass_pointer;

        case tkasm_unknown:
            default:
                return tkasmClass_unknown;
    }
}

TkasmType typeClass_toType(const TkasmTypeClass class, int32_t size)
{
    size = abs(size);
    switch (class)
    {
        case tkasmClass_uint:
            if (size == 64)
                return tkasm_uint64;
            else if (size == 32)
                return tkasm_uint32;
            else if (size == 16)
                return tkasm_uint16;
            else
                return tkasm_uint8;
            break;

        case tkasmClass_int:
            if (size == 64)
                return tkasm_int64;
            else if (size == 32)
                return tkasm_int32;
            else if (size == 16)
                return tkasm_int16;
            else
                return tkasm_uint8;
            break;

        case tkasmClass_float:
            if(size > 32)
                return tkasm_double;
            else
                return tkasm_float;
            break;

        case tkasmClass_array:
        case tkasmClass_pointer:
        case tkasmClass_unknown:
            break;
    }
    return tkasm_unknown;
}


TkasmType getBiggerType(const TkasmType type1, const TkasmType type2)
{
    if (type1 == tkasm_unknown || type2 == tkasm_unknown)
        return tkasm_unknown;

    if(type1 == tkasm_double || type2 == tkasm_double)
        return tkasm_double;

    if(type1 == tkasm_float || type2 == tkasm_float)
        return tkasm_float;

    const TkasmTypeClass typeClass1 = getTypeClass(&type1);
    const TkasmTypeClass typeClass2 = getTypeClass(&type2);

    const TkasmType biggerType = (TYPE_IS_BIGGER(&type1, &type2)) ? type1 : type2;

    if (typeClass1 > typeClass2)
        return typeClass_toType(typeClass1, getTypeSize(&biggerType));

    if (typeClass1 < typeClass2)
        return typeClass_toType(typeClass2, getTypeSize(&biggerType));

    return biggerType;
}

TkasmType getTypeOfArray(const char* type)
{
    if (STR_EQUALS(type, "%array.char"))
    {
        return tkasm_char;
    }

    else if (STR_EQUALS(type, "%array.uint64"))
    {
        return tkasm_uint64;
    }
    else if (STR_EQUALS(type, "%array.int64"))
    {
        return tkasm_int64;
    }

    else if (STR_EQUALS(type, "%array.uint32"))
    {
        return tkasm_uint32;
    }
    else if (STR_EQUALS(type, "%array.int32"))
    {

        return tkasm_int32;
    }
    else if (STR_EQUALS(type, "%array.uint16"))
    {
        return tkasm_uint16;
    }
    else if (STR_EQUALS(type, "%array.int16"))
    {
        return tkasm_int16;
    }

    else if (STR_EQUALS(type, "%array.uint8"))
    {
        return tkasm_uint8;
    }
    else if (STR_EQUALS(type, "%array.int8"))
    {
        return tkasm_int8;
    }

    else if (STR_EQUALS(type, "%array.float"))
    {
        return tkasm_float;
    }
    else if (STR_EQUALS(type, "%array.double"))
    {
        return tkasm_double;
    }
    return tkasm_unknown;
}

TkasmType getType(const char *type)
{
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

    else if (STR_EQUALS(type, "%array.char"))
    {
        return tkasm_arrayChar;
    }

    else if (STR_EQUALS(type, "%array.uint64"))
    {
        return tkasm_arrayUint64;
    }
    else if (STR_EQUALS(type, "%array.int64"))
    {
        return tkasm_arrayInt64;
    }

    else if (STR_EQUALS(type, "%array.uint32"))
    {
        return tkasm_arrayUint32;
    }
    else if (STR_EQUALS(type, "%array.int32"))
    {

        return tkasm_arrayInt32;
    }
    else if (STR_EQUALS(type, "%array.uint16"))
    {
        return tkasm_arrayUint16;
    }
    else if (STR_EQUALS(type, "%array.int16"))
    {
        return tkasm_arrayInt16;
    }

    else if (STR_EQUALS(type, "%array.uint8"))
    {
        return tkasm_arrayUint8;
    }
    else if (STR_EQUALS(type, "%array.int8"))
    {
        return tkasm_arrayInt8;
    }

    else if (STR_EQUALS(type, "%array.float"))
    {
        return tkasm_arrayFloat;
    }
    else if (STR_EQUALS(type, "%array.double"))
    {
        return tkasm_arrayDouble;
    }

    else if (STR_EQUALS(type, "%returnPointer"))
    {
        return tkasm_returnPointer;
    }
    return tkasm_unknown;
}

const char* getTypeString(const TkasmType *type)
{
    switch (*type)
    {
        case tkasm_char:
            return "%char";


        case tkasm_uint64:
            return "%uint64";

        case tkasm_int64:
            return "%int64";


        case tkasm_uint32:
            return "%uint32";

        case tkasm_int32:
            return "%int32";


        case tkasm_uint16:
            return "%uint16";

        case tkasm_int16:
            return "%int16";


        case tkasm_uint8:
            return "%uint8";

        case tkasm_int8:
            return "%int8";


        case tkasm_float:
            return "%float";

        case tkasm_double:
            return "%double";


        case tkasm_arrayChar:
            return "%array.char";


        case tkasm_arrayUint64:
            return "%array.uint64";

        case tkasm_arrayInt64:
            return "%array.int64";


        case tkasm_arrayUint32:
            return "%array.uint32";

        case tkasm_arrayInt32:
            return "%array.int32";


        case tkasm_arrayUint16:
            return "%array.uint16";

        case tkasm_arrayInt16:
            return "%array.int16";


        case tkasm_arrayUint8:
            return "%array.uint8";

        case tkasm_arrayInt8:
            return "%array.int8";


        case tkasm_arrayFloat:
            return "%array.float";

        case tkasm_arrayDouble:
            return "%array.double";

        case tkasm_returnPointer:
            return "%returnPointer";

        case tkasm_unknown:
            return "unknown";

        default:
            break;
    }

    return "null";
}

int16_t getTypeSize(const TkasmType *type)
{
    const TkasmTypeClass class = getTypeClass(type);
    if(class == tkasmClass_array)
        return -1;

    switch (*type)
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

void* convertToTypeClass(const TkasmType oldType, const TkasmType newType, void* value, /*out*/bool *isSuccess)
{
    const TkasmTypeClass newClass = getTypeClass(&newType);
    *isSuccess = true;


    switch(oldType)
    {
        case tkasm_char:
        {
            const char newValue = (char)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_int64:
        {
            const int64_t newValue = (int64_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_int32:
        {
            const int32_t newValue = (int32_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_int16:
        {
            const int16_t newValue = (int16_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_int8:
        {
            const int8_t newValue = (int8_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }


        case tkasm_uint64:
        {
            const uint64_t newValue = (uint64_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_uint32:
        {
            const uint32_t newValue = (uint32_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_uint16:
        {
            const uint16_t newValue = (uint16_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_uint8:
        {
            const uint8_t newValue = (uint8_t)value;
            RECAST_VALUE(newClass, newType, newValue);
        }

        case tkasm_float:
        {
            const float newValue = *(float*)&value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        case tkasm_double:
        {
            const double newValue = *(double*)&value;
            RECAST_VALUE(newClass, newType, newValue);
        }
        default:
            break;
    }

    *isSuccess = false;
    printf("!!<warning> convertToTypeClass did nothing oldType: %s to type: %s!!", getTypeString(&oldType), getTypeString(&newType));
    return value;
}

size_t getRawArraySize(const char* rawArray)
{
    size_t size;
    const char** splits = split_string((char*)trimWhiteSpaces(rawArray), ",", &size);
    free(splits);

    return size;
}