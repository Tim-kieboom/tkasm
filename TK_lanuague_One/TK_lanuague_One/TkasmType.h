#pragma once
#ifndef __TKASMTYPE_H__
#define __TKASMTYPE_H__

#include <stack>
#include "debug.h"

using namespace std;
typedef stack<uint8_t> Stack;

enum TkasmType
{
	tkasm_char,

	tkasm_uint64,
	tkasm_int64,

	tkasm_uint32,
	tkasm_int32,

	tkasm_uint16,
	tkasm_int16,

	tkasm_uint8,
	tkasm_int8,

	tkasm_unknown,
};

#endif
