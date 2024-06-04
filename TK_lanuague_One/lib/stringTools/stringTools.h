//
// Created by tim_k on 04/06/2024.
//

#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H
#pragma once
#include "../ArrayList/ArrayList.h"
#include "../Stack/Stack.h"
#include "../StringStream/StringStream.h"
#include "../String/String.h"

char** split_string(char* str, const char delim);
const char* uint32_toString(uint32_t value);

#endif //STRINGTOOLS_H
