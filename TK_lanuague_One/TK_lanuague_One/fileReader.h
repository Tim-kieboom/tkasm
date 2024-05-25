#ifndef __FILEREADER_H__
#define __FILEREADER_H__
#pragma once

#include <vector>
#include <string>

using namespace std;
vector<string>* readFile(const char* filePath);
vector<string>* readFile(char* filePath);

#endif 