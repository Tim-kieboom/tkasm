//
// Created by tim_k on 04/06/2024.
//

#ifndef STRING_H
#define STRING_H
#pragma once
#include <stdint.h>
#include "../ArrayList/ArrayList.h"

typedef struct string
{
    const char* data;
} String;

String *String_new(const char* string);

String *String_newFromInt(const uint32_t string);

uint64_t String_size(const String* string);

arraylist *splitWhiteSpaces(const String *str);
char String_at(const String* string, const uint32_t index);

void String_free(String* string);

#endif //STRING_H
