//
// Created by tim_k on 04/06/2024.
//
#include "String.h"

#include <stdio.h>
#include <stdlib.h>

const char* uint32_toString(uint32_t value)
{
    //get the size of the string
    const int lenght = snprintf(NULL, 0, "%d", value);
    char* str = malloc(lenght + 1);
    snprintf(str, lenght + 1, "%d", value);
    return str;
}

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
uint64_t String_size(const String* string)
{
    const char* data = string->data;
    uint64_t size = 0;
    while(*data != '\0')
    {
        size++;
        data++;
    }
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