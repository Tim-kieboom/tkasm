#pragma once
#ifndef __GENERALTYPES_H__
#define __GENERALTYPES_H__

#include "TkasmType.h"

//casts the raw data over to type
#define EXPLICIT_CAST(type, value) *(type*)&(value)
#define FLOAT_TO_VOIDP(value) ((void*)*(int32_t*)&value)
#define DOUBLE_TO_VOIDP(value) ((void*)*(size_t*)&value)

TkasmType getType(string& rawType);
const char* getTypeString(TkasmType& type);

//a negative value means signed value of size abs(value)
int16_t getTypeSize(TkasmType& type);
TkasmType getBiggerType(TkasmType type1, TkasmType type2);
TkasmTypeClass getTypeClass(TkasmType& type);

//------------------------------------------------------------

void pushType(TkasmType type, uint8_t* segments, Stack* stack);
uint8_t* popType(TkasmType type, Stack* stack);

uint8_t* segmentType(TkasmType& type, void* value);
void* unsegmentType(TkasmType& type, uint8_t* segments, /*out*/bool& isSuccess);

void* convertToTypeClass(TkasmType& oldType, TkasmType& newType, void* value, /*out*/bool& isSuccess);

#endif