//
// Created by tim_k on 05/06/2024.
//

#ifndef GENERALTYPE_H
#define GENERALTYPE_H
#pragma once
#include "../TkasmTypes.h"
#include "../../lib/string/String.h"
#include "../../lib/stringTools/stringTools.h"

#define EXPLICIT_CAST(type, value) *(type*)&(value)
#define FLOAT_TO_VOIDP(value) ((void*)*(int32_t*)value)
#define DOUBLE_TO_VOIDP(value) ((void*)*(size_t*)value)

TkasmTypeClass getTypeClass(const TkasmType *type);
TkasmType getBiggerType(const TkasmType type1, const TkasmType type2);
TkasmType getType(const char *type);
const char* getTypeString(const TkasmType *type);
int16_t getTypeSize(const TkasmType *type);

void* convertToTypeClass(const TkasmType oldType, const TkasmType newType, void* value, /*out*/bool *isSuccess);

#endif //GENERALTYPE_H
