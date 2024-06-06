//
// Created by tim_k on 04/06/2024.
//

#include "stringTools.h"
#define IS_WHITE_SPACE(ch) ((ch == ' ') || (ch == '\t') || (ch == '\n'))
#include <stdio.h>
#include <stdlib.h>

const char* trimWhiteSpaces(const char* str)
{
    StringStream *token = StringStream_new();
    bool isInString = false;

    size_t i = -1;
    while (str[++i] != '\0')
    {
        const char letter = str[i];

        if (letter == '\"')
            isInString = !isInString;

        if (!IS_WHITE_SPACE(letter) || isInString == true)
            StringStream_append(token, letter);
    }

    const char* strtoken = StringStream_toCharPtr(token);
    StringStream_free(token);
    return strtoken;
}

arraylist/*const char[]*/* splitWhiteSpaces(const char *str)
{
    StringStream *token = StringStream_new();
    arraylist/*const char[]*/ *tokens = arraylist_create();

    bool isInString = false;

    size_t i = -1;
    while (str[++i] != '\0')
    {
        const char letter = str[i];

        if (letter == '\"')
            isInString = !isInString;

        if (IS_WHITE_SPACE(letter) && (isInString == false))
        {
            if (token->size == 0)
                continue;

            const char* strToken = StringStream_toCharPtr(token);

            arraylist_add(tokens, (void*)strToken);
        }

        if (!IS_WHITE_SPACE(letter) || (isInString == true))
            StringStream_append(token, letter);
    }

    if (token->size != 0)
    {
        const char *strToken = StringStream_toCharPtr(token);
        arraylist_add(tokens, (void*)strToken);
    }

    StringStream_free(token);
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

const char* uint32_toString(const uint32_t value)
{
    //get the size of the string
    const int lenght = snprintf(NULL, 0, "%d", value);
    char* str = malloc(lenght + 1);
    snprintf(str, lenght + 1, "%d", value);
    return str;
}