//
// Created by tim_k on 04/06/2024.
//
#include "fileReader.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../StringStream/StringStream.h"

arraylist* readFile(const char* path)
{
    StringStream *stream = StringStream_new();
    arraylist *fileString = arraylist_create();

    FILE* file = fopen(path, "r");
    if(file == NULL)
    {
        printf("!!<error> coudn't open file!!");
        exit(1);
    }

    char letter;
    uint32_t i = 0;
    while((letter = fgetc(file)) != EOF)
    {
        if(letter == '\n' || letter == '\r')
        {
            const char* line = StringStream_toCharPtr(stream);
            arraylist_add(fileString, (char*)line);

            StringStream_free(stream);
            stream = StringStream_new();
            i++;
        }

        StringStream_append(stream, letter);
        i++;

    }

    fclose(file);
    StringStream_free(stream);
    return fileString;
}
