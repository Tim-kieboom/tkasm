#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "metaData.h"

using namespace std;

vector<string> tokenizer(vector<string>* lines, unordered_map<string, uint32_t>& labelTracker, unordered_map<uint32_t, uint32_t>& lineNumberTracker);
