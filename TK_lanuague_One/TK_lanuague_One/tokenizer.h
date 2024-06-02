#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <stack>


using namespace std;

vector<string> tokenizer(vector<string>* lines, /*out*/unordered_map<string, uint32_t>& labelTracker, /*out*/unordered_map<uint32_t, uint32_t>& lineNumberTracker);
