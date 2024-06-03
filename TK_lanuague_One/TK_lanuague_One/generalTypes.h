#pragma once
#ifndef __GENERALTYPES_H__
#define __GENERALTYPES_H__

#include "TkasmType.h"

//casts the raw data over to type
#define EXPLICIT_CAST(type, value) *(type*)&(value)

TkasmType getType(string& rawType);
const char* getTypeString(TkasmType& type);

//a negative value means signed value of size abs(value)
int16_t getTypeSize(TkasmType& type);
TkasmType getBiggerType(TkasmType type1, TkasmType type2);

//------------------------------------------------------------


void pushType(TkasmType type, uint8_t* segments, Stack* stack);
uint8_t* popType(TkasmType type, Stack* stack);

uint8_t* segmentType(TkasmType& type, void* value);
void* unsegmentType(TkasmType& type, uint8_t* segments, /*out*/bool& isSuccess);



void* readTypeFromConsole(TkasmType& type, /*out*/bool& isSuccess);
void printTypeToConsole(TkasmType& type, void* value);

void* stringToType(TkasmType type, string& rawValue, /*out*/bool& isSuccess);

bool isTypeEqual0(TkasmType& type, void* value);
bool isTypeGreater0(TkasmType& type, void* value);
bool isTypeSmaller0(TkasmType& type, void* value);

#endif