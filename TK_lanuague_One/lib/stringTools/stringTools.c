//
// Created by tim_k on 04/06/2024.
//

#include "stringTools.h"
#define IS_WHITE_SPACE(ch) ((ch == ' ') || (ch == '\t') || (ch == '\n'))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

arraylist/*strings*/* splitWhiteSpaces(const String *str)
{
    Stack/*chars*/ *token = Stack_create(NULL);
    arraylist/*strings*/ *tokens = arraylist_create();

    bool isInString = false;

    size_t i = -1;
    while (String_at(str, ++i) != '\0')
    {
        char letter = String_at(str, i);

        if (letter == '\"')
            isInString = !isInString;

        if (IS_WHITE_SPACE(letter) && (isInString == false))
        {
            if (Stack_isEmpty(token))
                continue;

            String *strToken = String_new((const char*)Stack_array(token));

            arraylist_add(tokens, strToken);
        }

        if (!IS_WHITE_SPACE(letter) || (isInString == true))
            Stack_push(token, (void*)letter);
    }

    if (!Stack_isEmpty(token))
    {
        String *strToken = String_new((const char*)Stack_array(token));
        arraylist_add(tokens, strToken);
    }

    Stack_free(token);
    return tokens;
}

char** split_string(char* str, const char delim)
{
    int token_count = 0;
    char* token = strtok(str, &delim);
    char** tokens = malloc(sizeof(char*));

    while (token != NULL)
    {
        tokens[token_count] = malloc(strlen(token) + 1);
        strcpy(tokens[token_count], token);
        token_count++;
        tokens = realloc(tokens, (token_count + 1) * sizeof(char*));
        token = strtok(NULL, &delim);
    }

    tokens[token_count] = NULL;
    return tokens;
}

const char* uint32_toString(uint32_t value)
{
    //get the size of the string
    const int lenght = snprintf(NULL, 0, "%d", value);
    char* str = malloc(lenght + 1);
    snprintf(str, lenght + 1, "%d", value);
    return str;
}