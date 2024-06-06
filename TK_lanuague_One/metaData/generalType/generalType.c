//
// Created by tim_k on 05/06/2024.
//

#include "generalType.h"

#include <stdlib.h>
#define TYPE_IS_BIGGER(type1, type2) (abs(getTypeSize(type1)) > abs(getTypeSize(type2)))

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

        case tkasm_unknown:
            default:
                return tkasmClass_unknown;
    }
}


TkasmType getBiggerType(const TkasmType type1, const TkasmType type2)
{
    const TkasmTypeClass typeClass1 = getTypeClass(&type1);
    const TkasmTypeClass typeClass2 = getTypeClass(&type2);

    if (typeClass1 > typeClass2)
    {
        return type1;
    }
    else if (typeClass1 < typeClass2)
    {
        return type2;
    }

    if (TYPE_IS_BIGGER(&type1, &type2))
        return type1;

    return type2;
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

        case tkasm_unknown:
            return "unknown";

        default:
            break;
    }

    return "null";
}

int16_t getTypeSize(const TkasmType *type)
{
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