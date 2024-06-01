#pragma once
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <iostream>
#include <string>
#include <vector>
#include "Types.h"

using namespace std;

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

    tkasm_print,

    tkasm_printPop,
    tkasm_read,

    tkasm_jump,

    tkasm_jumpEquals0,
    tkasm_jumpGreater0,

    tkasm_halt,
};

TKasmCommand getCommand(const char* command);

void read(/*out*/Stack* stack, string& rawType, DebugData* data);
void printPop(/*out*/Stack* stack, string& rawType, DebugData* data);

#endif