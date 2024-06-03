#pragma once
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
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
	tkasm_jumpSmaller0,

	tkasm_halt,
};

TKasmCommand getCommand(const char* command);

void push(/*out*/Stack* stack, string& type, string& value, DebugData* data);
void pop(/*out*/Stack* stack, string& rawType, DebugData* data);

void add(/*out*/Stack* stack, string& rawType1, string rawType2, DebugData* data);
void sub(/*out*/Stack* stack, string& rawType1, string& rawType2, DebugData* data);

void print(string& rawValue, DebugData* data);

void printPop(/*out*/Stack* stack, string& rawType, DebugData* data);
void read(/*out*/Stack* stack, string& rawType, DebugData* data);

void jump(/*out*/uint32_t& index, unordered_map<string, uint32_t>& labelTracker, string& label);
bool isEquals0(/*out*/Stack* stack, string& rawType, DebugData* data);
bool isGreater0(/*out*/Stack* stack, string& rawType, DebugData* data);
bool isSmaller0(/*out*/Stack* stack, string& rawType, DebugData* data);

#endif