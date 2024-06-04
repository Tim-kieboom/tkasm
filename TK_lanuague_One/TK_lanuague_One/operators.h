#pragma once
#ifndef __OPERATORS_H__
#define __OPERATORS_H__

#include "TkasmType.h"

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);
void* subTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);

void* mullTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);
void* divTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool& isSuccess, DebugData* data);

bool isTypeEqual0(TkasmType& type, void* value);
bool isTypeGreater0(TkasmType& type, void* value);
bool isTypeSmaller0(TkasmType& type, void* value);

#endif 


