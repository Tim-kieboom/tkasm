#pragma once
#ifndef __stack_H__
#define __stack_H__

#include "TkasmType.h"

uint8_t* segmentType(TkasmType& type, void* value);
void* unsegmentType(TkasmType& type, uint8_t* segments, /*out*/bool& isSuccess);

uint8_t* popType(TkasmType type, Stack* stack);
void pushType(TkasmType type, uint8_t* segments, Stack* stack);

#endif 


