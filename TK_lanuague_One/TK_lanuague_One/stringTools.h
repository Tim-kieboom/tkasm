#ifndef __STRINGTOOLS_H__
#define __STRINGTOOLS_H__
#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int16_t stringTo_int16(char* string);
int16_t stringTo_int16(const char* string);

int32_t stringTo_int32(char* str);
int32_t stringTo_int32(const char* str);

vector<string> splitString(string &str, char delimiter);

int32_t sizeofString(char* string);
int32_t sizeofString(const char* string);

char stringLastChar(char* string);
char stringLastChar(const char* string);

#endif




