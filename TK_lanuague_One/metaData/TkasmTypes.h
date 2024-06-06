//
// Created by tim_k on 04/06/2024.
//

#ifndef TKASMTYPES_H
#define TKASMTYPES_H
#pragma once

enum TkasmType
{
    tkasm_char,

    tkasm_uint64,
    tkasm_int64,

    tkasm_uint32,
    tkasm_int32,

    tkasm_uint16,
    tkasm_int16,

    tkasm_uint8,
    tkasm_int8,

    tkasm_float,
    tkasm_double,

    tkasm_unknown,
};

//number don't matter but THE ORDER DOES!!
enum TkasmTypeClass
{
    tkasmClass_uint = 1,
    tkasmClass_int = 2,
    tkasmClass_float = 3,
    tkasmClass_array = 4,
    tkasmClass_unknown = 5
};

typedef enum TkasmType TkasmType;
typedef enum TkasmTypeClass TkasmTypeClass;

#endif //TKASMTYPES_H
