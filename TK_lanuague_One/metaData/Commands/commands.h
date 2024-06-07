//
// Created by tim_k on 05/06/2024.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#pragma once
#include "../../lib/HashMap/map.h"
#include "../debug/debug.h"
#include "../memory/stack/stackMemory.h"

enum TKasmCommand
{
    tkasm_push,
    tkasm_pop,

    tkasm_movPush,
    tkasm_movPop,
    tkasm_mov,

    tkasm_free,

    tkasm_add,
    tkasm_sub,

    tkasm_mull,
    tkasm_div,

    tkasm_print,

    tkasm_printPop,
    tkasm_read,

    tkasm_jump,

    tkasm_jumpEquals0,
    tkasm_jumpGreater0,
    tkasm_jumpSmaller0,

    tkasm_halt,
};

typedef enum TKasmCommand TKasmCommand;

TKasmCommand getCommand(const char* command);

void tk_push(/*out*/Stack* stack, const char* rawType, const char* rawValue, const DebugData* data);
void tk_pop(/*out*/Stack* stack, const char* rawType, DebugData* data);

void tk_add(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data);
void tk_sub(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data);

void tk_mull(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data);
void tk_div(/*out*/Stack* stack, const char* rawType1, const char* rawType2, const DebugData* data);

void tk_print(const char* rawValue, DebugData* data);
void tk_printPop(/*out*/Stack* stack, const char* rawType, const DebugData* data);

void tk_read(/*out*/Stack* stack, const char* rawType, const DebugData* data);

void tk_jump(/*out*/uint32_t *index, map_int_t *labelTracker, const char* label);

bool tk_isEquals0(/*out*/Stack* stack, const char* rawType, const DebugData* data);
bool tk_isGreater0(/*out*/Stack* stack, const char* rawType, const DebugData* data);
bool tk_isSmaller0(/*out*/Stack* stack, const char* rawType, const DebugData* data);

#endif //COMMANDS_H
