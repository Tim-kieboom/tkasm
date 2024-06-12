//
// Created by tim_k on 04/06/2024.
//
#include "String.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../lib/stringTools/stringTools.h"

String *String_new(const char* string)
{
    String *str = malloc(sizeof(String));
    str->data = string;
    return str;
}

String *String_newFromInt(const uint32_t value)
{
    return String_new(uint32_toString(value));
}

uint64_t String_size(const char* string)
{
    const char* data = string;

    if(string == NULL || string[0] == '\0')
        return 0;

    uint64_t size = -1;
    while(data[++size] != '\0');

    return size;
}

char String_at(const String* string, const uint32_t index)
{
    return string->data[index];
}

void String_free(String* string)
{
    if(string == NULL)
        return;

    if(string->data != NULL)
        free((char*)string->data);

    free(string);

}