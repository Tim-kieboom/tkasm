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

    stream->list = arraylist_create();
	return stream;
}

void StringStream_append(const StringStream *stringStream, const char letter)
{
    arraylist_add(stringStream->list, (void*)letter);
}

const char* StringStream_toCharPtr(const StringStream *stringStream)
{
    arraylist_add(stringStream->list, '\0');
    char* string = malloc(stringStream->list->size * sizeof(char));

    uint32_t i = -1;
    while((char)arraylist_get(stringStream->list, ++i) != '\0')
    {
        string[i] = (char)arraylist_get(stringStream->list, i);
    }

    return string;
}

String* StringStream_toString(const StringStream *stringStream)
{
    arraylist_add(stringStream->list, '\0');
    char* string = malloc(stringStream->list->size * sizeof(char));

    uint32_t i = -1;
    while((char)arraylist_get(stringStream->list, ++i) != '\0')
    {
        string[i] = (char)arraylist_get(stringStream->list, i);
    }

    return String_new(string);
}
void StringStream_free(StringStream *stringStream)
{
    if(stringStream == NULL)
        return;

    if(stringStream->list != NULL)
        arraylist_destroy(stringStream->list);
    
    free(stringStream);
}
