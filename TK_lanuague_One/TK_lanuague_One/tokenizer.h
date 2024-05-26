#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

enum TkasmType
{
    tkasm_char,
    tkasm_int,
};

enum TKasmCommand
{
    tkasm_push,
    tkasm_pop,

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

typedef struct DebugData
{
    int32_t currentLine = 0;
    string commandName = "";
};

string stringOfDebugData(DebugData* data);
TKasmCommand getCommand(const char* command);
void checkIfStackIsEmpty(stack<int32_t>* st, DebugData* data);
void checkIfLineHasValue(size_t PartsSize, int32_t lineNumber);
void checkIfCommandHasType(vector<string> parts, int32_t lineNumber);
vector<string> tokenizer(vector<string>* lines, unordered_map<string, int32_t>& labelTracker, unordered_map<int32_t, int32_t>& lineNumberTracker);
