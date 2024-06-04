#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H
#pragma once

#include "../ArrayList/ArrayList.h"
#include "../String/String.h"

typedef struct stringStream
{
    arraylist *list;
} StringStream;

StringStream* StringStream_new();

void StringStream_append(const StringStream *stringStream, char letter);
const char* StringStream_toCharPtr(StringStream *stringStream);

String* StringStream_toString(const StringStream *stringStream);

void StringStream_free(StringStream *stringStream);
#endif