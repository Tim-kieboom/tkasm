#pragma once
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "generalTypes.h"

void* stringToType(TkasmType type, string& rawValue, /*out*/bool& isSuccess);
void* readTypeFromConsole(TkasmType& type, /*out*/bool& isSuccess);
void printTypeToConsole(TkasmType& type, void* value);

#endif