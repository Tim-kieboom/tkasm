#pragma once
#ifndef  __TYPES_H__
#define  __TYPES_H__

#include <iostream>
#include <stack>
#include "debug.h"

using namespace std;

typedef stack<uint8_t> Stack;

//casts the raw data over to type
#define EXPLICIT_CAST(type, value) *(type*)&(value)

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

	tkasm_unknown,
};

TkasmType getType(string& rawType);
const char* getTypeString(TkasmType& type);

//a negative value means signed value of size abs(value)
int16_t getTypeSize(TkasmType& type);

//------------------------------------------------------------
TkasmType getBiggerType(TkasmType type1, TkasmType type2);

void pushType(TkasmType type, uint8_t* segments, Stack* stack);
uint8_t* popType(TkasmType type, Stack* stack);

uint8_t* segmentType(TkasmType& type, void* value);
void* unsegmentType(TkasmType& type, uint8_t* segments, /*out*/bool& isSuccess);

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);
void* subTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);

void* readTypeFromConsole(TkasmType& type, /*out*/bool& isSuccess);
void printTypeToConsole(TkasmType& type, void* value);

void* stringToType(TkasmType type, string& rawValue, /*out*/bool& isSuccess);

bool isTypeEqual0(TkasmType& type, void* value);
bool isTypeGreater0(TkasmType& type, void* value);
bool isTypeSmaller0(TkasmType& type, void* value);


#endif


