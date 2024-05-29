#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "types.h"

using namespace std;

struct DebugData
{
    int32_t currentLine;
    string commandName;

    DebugData();
};

bool isLineCommand(string line);
string stringOfDebugData(DebugData* data);
void exit_LineHasNoValue(uint32_t lineNumber);

void checkIfStackIsEmpty(Stack* st, DebugData* data);
void checkIfCommandHasType(vector<string> parts, uint32_t lineNumber);
void checkIfTypeIsValid(TkasmType& type, string& rawType, DebugData* data);

#endif

