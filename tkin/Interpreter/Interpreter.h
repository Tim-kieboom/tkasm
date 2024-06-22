//
// Created by tim_k on 06/06/2024.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H
#pragma once

#include "../tokenizer/mainTokenizer.h"
#include "../lib/arraylist/arraylist.h"
#include "../metaData/debug/debug.h"
#include "../lib/HashMap/map.h"
#include "../lib/Stack/Stack.h"

void interpretFile_andExit(arraylist/*const char[]*/* lines);

arraylist* interpretFile(uint32_t *i, arraylist/*const char[]*/* lines, /*out*/Stack* stack, const TokenizeData *tokenizeData, DebugData *debugData);
void interpretLine(arraylist* program, /*out*/uint32_t *i, /*out*/Stack* stack, map_str_t *labelTracker, map_int_t *lineNumberTracker, DebugData *debugData);

#endif //INTERPRETER_H
