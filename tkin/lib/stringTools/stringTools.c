//
// Created by tim_k on 04/06/2024.
//

#include "stringTools.h"
#include <stdio.h>
#include <stdlib.h>

#define IS_WHITE_SPACE(ch) ((ch == ' ') || (ch == '\t') || (ch == '\n'))
#define IS_IN_QUOTES(str, begin, end) (str[begin] == '\"' && str[end] == '\"')

const char* trimWhiteSpaces(const char* str)
{
    StringStream *token = StringStream_new();
    bool isInString = false;

    size_t i = -1;
    while (str[++i] != '\0')
    {
        const char letter = str[i];

        if (letter == '"' && str[i - 1] != '\\')
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
    bool isInArray = false;

    size_t i = -1;
    while (str[++i] != '\0')
    {
        const char letter = str[i];

        if (letter == '\"' && str[i - 1] != '\\')
            isInString = !isInString;

        if(letter == '{')
            isInArray = true;

        if(letter == '}')
            isInArray = false;

        if (IS_WHITE_SPACE(letter) && !isInString && !isInArray)
        {
            if (token->size == 0)
                continue;

            const char* strToken = StringStream_toCharPtr(token);

            arraylist_add(tokens, (void*)strToken);
        }
        else
        {
            StringStream_append(token, letter);
        }
    }

    if (token->size != 0)
    {
        const char *strToken = StringStream_toCharPtr(token);
        arraylist_add(tokens, (void*)strToken);
    }

    StringStream_free(token);
    return tokens;
}

const char** split_string(const char* str, const char *delim, /*out*/size_t *size)
{
    StringStream *stream = StringStream_new();
    arraylist *tokens = arraylist_create();

    uint32_t i = -1;
    while(str[++i] != '\0')
    {
        const char letter = str[i];

        if (strchr(delim, letter) != NULL)
        {
            if(stream->size > 0)
                arraylist_add(tokens, (void*)StringStream_toCharPtr(stream));
            continue;
        }

        StringStream_append(stream, letter);
    }

    if(stream->size > 0)
        arraylist_add(tokens, (void*)StringStream_toCharPtr(stream));

    StringStream_free(stream);
    const char** result = (const char**)tokens->body;
    *size = tokens->size;
    free(tokens);
    return result;
}

const char* getInternalString(const char* str)
{
    StringStream *stream = StringStream_new();
    arraylist *tokens = arraylist_create();

    uint32_t i = -1;
    while(str[++i] != '\0')
    {
        const char letter = str[i];
        const char prevLetter = (i == 0) ? '\0' : str[i - 1];

        if (letter == '\"' && prevLetter != '\\')
        {
            if(stream->size > 0)
                arraylist_add(tokens, (void*)StringStream_toCharPtr(stream));
            continue;
        }

        StringStream_append(stream, letter);
    }

    if(stream->size > 0)
        arraylist_add(tokens, (void*)StringStream_toCharPtr(stream));

    const char** result = (const char**)tokens->body;
    const char* resultString = result[0];
    StringStream_free(stream);
    arraylist_destroy(tokens);
    return resultString;
}

const char* uint32_toString(const uint32_t value)
{
    //get the size of the string
    const int lenght = snprintf(NULL, 0, "%d", value);
    char* str = malloc(lenght + 1);
    snprintf(str, lenght + 1, "%d", value);
    return str;
}

const char* parse_backslash(const char* string)
{
    StringStream *stream = StringStream_new();
    uint32_t i = -1;
    while(string[++i] != '\0')
    {
        if(string[i] != '\\')
        {
            StringStream_append(stream, string[i]);
            continue;
        }

        if(string[i+1] == '\0')
            break;

        const char backLetter = string[i+1];

        switch (backLetter)
        {
            case 'r':
                StringStream_append(stream, '\r');
                break;
            case 'n':
                StringStream_append(stream, '\n');
                break;
            case 't':
                StringStream_append(stream, '\t');
                break;
            case '"':
                StringStream_append(stream, '\"');
                break;
            default:
                StringStream_append(stream, '\\');
        }

        i++;
    }

    const char* returnString = StringStream_toCharPtr(stream);
    StringStream_free(stream);
    return returnString;
}

bool isInternalString(const char* value)
{
    const char* trimmedValue = trimWhiteSpaces(value);
    const size_t size = strlen(trimmedValue);

    if(size < 2)
        return false;

    return IS_IN_QUOTES(trimmedValue, 0, size - 1);
}

