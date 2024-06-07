//
// Created by tim_k on 06/06/2024.
//

#include "Interpreter.h"

#include <stdio.h>
#include <stdlib.h>

#include "../metaData/debug/debug.h"
#include "../lib/HashMap/map.h"
#include "../lib/Stack/Stack.h"
#include "../tokenizer/mainTokenizer.h"
#include "../metaData/Types.h"
#include "../metaData/Commands/commands.h"

#define NEW_MAP(map_t)							\
({												\
	map_t *map = malloc(sizeof(map_t));			\
	map_init(map);								\
	map;										\
})
/*
instructions needed (/../ means done in tokenizer):
	push %type (value):                 push stack
	pop /%type/:                        pop stack

	mov.push (name):                    move value in heap to stack
	mov.pop /%type/ (name):             move top of stack to heap
	mov %type (name) (value):           move value to heap

	free (name):                        free value to heap

	add /%type/ /%type/:                pop 2 numbers from the stack and sum push the sum
	sub /%type/ /%type/:                pop 2 numbers and push the subtraction

	mul /%type /%type/:					pop 2 numbers from the stack and sum push the mul
	div /%type/ /%type/:				pop 2 numbers from the stack and sum push the div

	print "value":                      print const char*_literal to terminal
	print.pop /%type/:                  print the top of the stack

	read %type:                         read number from io input and push it to stack

	jump (label):                       jump to label
	jump.equals.0 /%type/ (label):      jump to label if top of stack is 0
	jump.greater.0 /%type/ (label):     jump to label if top of stack is greater then 0
	jump.smaller.0 /%type/ (label):     jump to label if top of stack is smaller then 0

	halt:                               stop program

*/

void exit_TypeNotFound(const char* rawType, DebugData* data)
{
	printf("!!<error> type[ %s ] not a valid type %s", rawType, StringOfDebugData(data));
	exit(1);
}

void exit_NotImplemented(const char* command, DebugData* data)
{
	printf("!!<error> command[ %s ] not implemented %s", command, StringOfDebugData(data));
	exit(1);
}

void printTokenized(const arraylist *lines, const arraylist *program, map_int_t *labelTracker)
{
	printf("file: %d lines\n", lines->size);
	printf("\n============================== FILE ==============================\n\n");
	const char* token;
	int it;
	arraylist_iterate(lines, it, token)
	{
		printf("%s", token);
	}
	printf("\n");


	printf("\n============================== TOKENIZER ==============================\n\n");
	arraylist_iterate(program, it, token)
	{
		printf("%s\n", token);
	}
	printf("\n============================================================\n\n");

}
void interpretCode(arraylist/*const char[]*/* lines)
{
	map_int_t *labelTracker = NEW_MAP(map_int_t);
	map_int_t *lineNumberTracker = NEW_MAP(map_int_t);

	arraylist* program = tokenizer(lines, labelTracker, lineNumberTracker);

	Stack *stack = Stack_new(10000000);
	DebugData *debugData = DebugData_new("null", -1);

	printTokenized(lines, program, labelTracker);

	uint32_t i = 0;
	while (strcmp(arraylist_get(program, i), "halt") != 0)
	{
		const char* command = arraylist_get(program, i);
		debugData->commandName = command;
		i++;

		const int* value = map_get(lineNumberTracker, uint32_toString(i));
		if(value != NULL)
			debugData->currentLine = *value;

		const TKasmCommand TkCommand = getCommand(command);

		switch (TkCommand)
		{
		case tkasm_push:
			tk_push(/*out*/stack, arraylist_get(program, i), arraylist_get(program, i+1), debugData);
			i += 2;
			break;

		case tkasm_pop:
			checkIfStackIsEmpty(stack, debugData);
			tk_pop(/*out*/stack, arraylist_get(program, i), debugData);
			i++;
			break;

		case tkasm_movPush:
			exit_NotImplemented(command, debugData);

			i++;
			break;

		case tkasm_movPop:
			exit_NotImplemented(command, debugData);

			i += 2;
			break;

		case tkasm_mov:
			exit_NotImplemented(command, debugData);

			i += 3;
			break;

		case tkasm_free:
			exit_NotImplemented(command, debugData);

			i++;
			break;

		case tkasm_add:
			checkIfStackIsEmpty(stack, debugData);
			tk_add(/*out*/stack, arraylist_get(program, i), arraylist_get(program, i+1), debugData);
			i += 2;
			break;

		case tkasm_sub:
			checkIfStackIsEmpty(stack, debugData);
			tk_sub(/*out*/stack, arraylist_get(program, i), arraylist_get(program, i+1), debugData);
			i += 2;
			break;

		case tkasm_mull:
			checkIfStackIsEmpty(stack, debugData);
			tk_mull(/*out*/stack, arraylist_get(program, i), arraylist_get(program, i+1), debugData);
			i += 2;
			break;

		case tkasm_div:
			checkIfStackIsEmpty(stack, debugData);
			tk_div(/*out*/stack, arraylist_get(program, i), arraylist_get(program, i+1), debugData);
			i += 2;
			break;

		case tkasm_print:
			tk_print(arraylist_get(program, i), debugData);
			i++;
			break;

		case tkasm_printPop:
			tk_printPop(/*out*/stack, arraylist_get(program, i), debugData);
			i++;
			break;

		case tkasm_read:
			tk_read(/*out*/stack, arraylist_get(program, i), debugData);
			i++;
			break;

		case tkasm_jump:
			tk_jump(/*out*/&i, labelTracker, arraylist_get(program, i));
			break;

		case tkasm_jumpEquals0:
			if (tk_isEquals0(stack, arraylist_get(program, i), debugData))
			{
				tk_jump(/*out*/&i, labelTracker, arraylist_get(program, i+1));
				continue;
			}
			i += 2;
			break;

		case tkasm_jumpGreater0:
			if (tk_isGreater0(/*out*/stack, arraylist_get(program, i), debugData))
			{
				tk_jump(/*out*/&i, labelTracker, arraylist_get(program, i+1));
				continue;
			}

			i += 2;
			break;

		case tkasm_jumpSmaller0:
			if (tk_isSmaller0(/*out*/stack, arraylist_get(program, i), debugData))
			{
				tk_jump(/*out*/&i, labelTracker, arraylist_get(program, i+1));
				continue;
			}

			i += 2;
			break;

		default:
			break;
		}
	}

	map_deinit(labelTracker);
	map_deinit(lineNumberTracker);
	free(labelTracker);
	free(lineNumberTracker);
}