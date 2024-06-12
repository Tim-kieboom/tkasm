#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H
#pragma once

#include "../Stream/Stream.h"
#include "../String/String.h"

typedef struct stringStream
{
    Stream *list;
    uint32_t size;
} StringStream;

StringStream* StringStream_new();

void StringStream_append(StringStream *stringStream, char letter);
void StringStream_appendCharPtr(StringStream *stringStream, const char* word);

String* StringStream_toString(StringStream *stringStream);
const char* StringStream_toCharPtr(StringStream *stringStream);
void StringStream_appendCharPtr_From(StringStream *stringStream, const char* word, const uint64_t begin);
void StringStream_appendCharPtr_Span(StringStream *stringStream, const char* word, const uint64_t begin, const uint64_t end);

void StringStream_free(StringStream *stringStream);
#endif