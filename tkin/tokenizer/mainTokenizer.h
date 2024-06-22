//
// Created by tim_k on 04/06/2024.
//

#ifndef MAINTOKENIZER_H
#define MAINTOKENIZER_H
#pragma once

#include "../lib/ArrayList/ArrayList.h"
#include "../lib/HashMap/map.h"
#include "../lib/Stream/Stream.h"

typedef struct tokenizeData
{
    Stream/*const char[]*/ *typeStack;
    Stream/*size_t*/ *arraySizeStack;
    map_str_t *labelTracker;
    map_int_t *lineNumberTracker;

} TokenizeData;

TokenizeData *TokenizeData_new();
void TokenizeData_free(TokenizeData *data);

arraylist/*strings*/ *tokenizeFile(arraylist/*strings*/* lines, /*out*/const TokenizeData *tokenizeData);

arraylist/*const char[]*/ *tokenizeLine(const char* line, /*out*/uint32_t *i, /*out*/arraylist *tokenizeHistory, /*out*/const TokenizeData *data);


#endif //MAINTOKENIZER_H
