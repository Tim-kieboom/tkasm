//
// Created by tim_k on 05/06/2024.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#pragma once
#include "../../lib/HashMap/map.h"
#include "../debug/debug.h"

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

/*void push(/*out#1#Stack* stack, const char* type, const char* value, DebugData* data);
void pop(/*out#1#Stack* stack, const char* rawType, DebugData* data);

void add(/*out#1#Stack* stack, const char* rawType1, String rawType2, DebugData* data);
void sub(/*out#1#Stack* stack, const char* rawType1, const char* rawType2, DebugData* data);

void mull(/*out#1#Stack* stack, const char* rawType1, const char* rawType2, DebugData* data);
void div(/*out#1#Stack* stack, const char* rawType1, const char* rawType2, DebugData* data);

void print(const char* rawValue, DebugData* data);

void printPop(/*out#1#Stack* stack, const char* rawType, DebugData* data);
void read(/*out#1#Stack* stack, const char* rawType, DebugData* data);

void jump(/*out#1#uint32_t *index, map_int_t *labelTracker, const char* label);
bool isEquals0(/*out#1#Stack* stack, const char* rawType, DebugData* data);
bool isGreater0(/*out#1#Stack* stack, const char* rawType, DebugData* data);
bool isSmaller0(/*out#1#Stack* stack, const char* rawType, DebugData* data);*/
#endif //COMMANDS_H
