//
// Created by tim_k on 04/06/2024.
//

#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H
#pragma once
#include "../ArrayList/ArrayList.h"
#include "../Stream/Stream.h"
#include "../StringStream/StringStream.h"
#include "../String/String.h"
#include <string.h>

#define STR_EQUALS(str1, str2) (strcmp(str1, str2) == 0)

const char* trimWhiteSpaces(const char* str);
arraylist/*strings*/* splitWhiteSpaces(const char *str);
const char** split_string(const char* str, const char *delim, /*out*/size_t *size);
const char* uint32_toString(uint32_t value);

const char* parse_backslash(const char* string);
const char* getInternalString(const char* str);

bool isInternalString(const char* value);

#endif //STRINGTOOLS_H
