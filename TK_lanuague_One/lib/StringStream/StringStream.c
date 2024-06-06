#include "StringStream.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void StringStream_free(StringStream *stringStream);

StringStream* StringStream_new()
{
    StringStream *stream = malloc(sizeof(StringStream));
	
	if (stream == NULL)
		return NULL;

    stream->list = Stack_create(NULL);
    stream->size = 0;
	return stream;
}

void StringStream_append(StringStream *stringStream, const char letter)
{
    Stack_push(stringStream->list, (void*)letter);
    stringStream->size++;
}

void StringStream_appendCharPtr(StringStream *stringStream, const char* word)
{
    int32_t i = -1;
    while(word[++i] != '\0')
        StringStream_append(stringStream, word[i]);
}

const char* StringStream_toCharPtr(StringStream *stringStream)
{
    if(stringStream->size == 0)
        return "\0";

    char* string = malloc((stringStream->size+1) * sizeof(char));
    string[stringStream->size] = '\0';

    while(stringStream->size != 0)
    {
        const char ch = (char)Stack_pop(stringStream->list);

        string[stringStream->size-1] = ch;
        stringStream->size--;
    }

    stringStream->size = 0;
    return string;
}

String* StringStream_toString(StringStream *stringStream)
{
    return String_new(StringStream_toCharPtr(stringStream));
}

void StringStream_free(StringStream *stringStream)
{
    if(stringStream == NULL)
        return;

    if(stringStream->list != NULL)
        Stack_free(stringStream->list);
    
    free(stringStream);
}
