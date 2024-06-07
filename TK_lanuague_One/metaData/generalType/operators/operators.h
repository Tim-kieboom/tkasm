//
// Created by tim_k on 06/06/2024.
//

#ifndef OPERATORS_H
#define OPERATORS_H
#pragma once

#include "../../TkasmTypes.h"
#include "../../debug/debug.h"
#include "../../memory/stack/stackMemory.h"
#include "../../../lib/Stack/Stack.h"

void* addTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data);
void* subTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data);
void* mullTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data);
void* divTypes(TkasmType type1, TkasmType type2, /*out*/Stack* stack, /*out*/bool *isSuccess, const DebugData* data);

bool isTypeEqual0(const TkasmType *type, void* value);
bool isTypeGreater0(const TkasmType *type, void* value);
bool isTypeSmaller0(const TkasmType *type, void* value);

#endif //OPERATORS_H
