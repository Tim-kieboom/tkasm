#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

struct DebugData
{
	int32_t currentLine;
	string commandName;

	DebugData();
	DebugData(string commandName, int32_t currentLine);
};

bool isLineCommand(string line);
string stringOfDebugData(DebugData* data);

void exit_LineHasNoValue(uint32_t lineNumber);
void exit_TypeIsNotValid(string& rawType, DebugData* data);
void exit_stackIsEmpty(DebugData* data);
void exit_SegmentationFailed(string& rawType, DebugData* data);

void checkIfStackIsEmpty(stack<uint8_t>* st, DebugData* data);
void checkIfCommandHasType(vector<string> parts, uint32_t lineNumber);


#endif

