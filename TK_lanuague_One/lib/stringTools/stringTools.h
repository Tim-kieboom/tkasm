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
#include <string.h>

#define STR_EQUALS(str1, str2) (strcmp(str1, str2) == 0)

const char* trimWhiteSpaces(const char* str);
arraylist/*strings*/* splitWhiteSpaces(const char *str);
char** split_string(char* str, const char delim);
const char* uint32_toString(uint32_t value);

#endif //STRINGTOOLS_H
