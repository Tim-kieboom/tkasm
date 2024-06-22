//
// Created by tim_k on 06/06/2024.
//

#ifndef STACKMEMORY_H
#define STACKMEMORY_H
#pragma once

#include <stdint.h>
#include "../../Types.h"
#include "../../../lib/Stack/Stack.h"

void removeAmountStack(const size_t amount, /*out*/Stack* stack);

void pushType(const TkasmType *type, const uint8_t* segments, /*out*/Stack* stack);
uint8_t* popType(const TkasmType *type, /*out*/Stack* stack);

uint8_t* peekType(const TkasmType *type, const Stack* stack);
uint8_t* peekArrayIndexType(const TkasmType *type, const Stack* stack, const uint32_t index);

uint8_t* segmentType(const TkasmType *type, void* value);
void* unsegmentType(const TkasmType *type, const uint8_t* segments, /*out*/bool *isSuccess);

#endif //STACKMEMORY_H
