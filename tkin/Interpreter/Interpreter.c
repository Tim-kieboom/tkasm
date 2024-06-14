//
// Created by tim_k on 06/06/2024.
//

#include "Interpreter.h"

#include <stdio.h>
#include <stdlib.h>

#include "../metaData/Types.h"
#include "../metaData/Commands/commands.h"

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

	shift.left /%type/ amount:			pops the top of the stack and bitshift left "amount" times
	shift.right /%type/ amount:			pops the top of the stack and bitshift right "amount" times

	print "value":                      print const char*_literal to terminal
	print.pop /%type/:                  print the top of the stack

	read %type:                         read number from io input and push it to stack

	jump (label):                       jump to label
	jump.equals.0 /%type/ (label):      jump to label if top of stack is 0
	jump.greater.0 /%type/ (label):     jump to label if top of stack is greater then 0
	jump.smaller.0 /%type/ (label):     jump to label if top of stack is smaller then 0

	halt:                               stop program

*/

void printTokenized(const arraylist *lines, const arraylist *program, map_int_t *labelTracker)
{
	printf("file: %d lines\n", (int)lines->size);
	printf("\n============================== FILE ==============================\n\n");
	char* token;
	int it;
	arraylist_iterate(lines, it, token)
	{
		if(it == 0)
		{
			printf("%d.\t ", it+1);
			printf("%s\n", token);
			continue;
		}

		token[0] = ' ';
		printf("%d.\t%s\n", it+1, token);
	}
	printf("\n");

	if(program->size == 0)
		return;

	printf("\n============================== TOKENIZER ==============================\n\n");
	arraylist_iterate(program, it, token)
	{
		printf("%d.\t%s\n", it+1, token);
	}
	printf("\n============================================================\n\n");

}

void interpretLine(arraylist* program, /*out*/uint32_t *i, /*out*/Stack* stack, map_int_t *labelTracker, map_int_t *lineNumberTracker, DebugData *debugData);

arraylist* interpretFile(uint32_t *i, arraylist/*const char[]*/* lines, /*out*/Stack* stack, TokenizeData *tokenizeData, DebugData *debugData)
{
	arraylist* program = tokenizeFile(lines, tokenizeData);
	printTokenized(lines, program, tokenizeData->labelTracker);

	*i = 0;
	while (strcmp(arraylist_get(program, *i), "halt") != 0)
	{
		interpretLine(program, i, stack, tokenizeData->labelTracker, tokenizeData->lineNumberTracker, debugData);

		if (program->size-1 < *i)
		{
			printf("!!<warning> program ended \'halt\' never reached!!\n");
			exit(0);
		}
	}

	return program;
}


void interpretFile_andExit(arraylist/*const char[]*/* lines)
{
	TokenizeData *tokenizeData = TokenizeData_new();

	arraylist* program = tokenizeFile(lines, tokenizeData);

	Stack *stack = Stack_new(1000000000);
	DebugData *debugData = DebugData_new("null", -1);

	printTokenized(lines, program, tokenizeData->labelTracker);

	uint32_t i = 0;
	while (strcmp(arraylist_get(program, i), "halt") != 0)
	{
		interpretLine(program, &i, stack, tokenizeData->labelTracker, tokenizeData->lineNumberTracker, debugData);

		if (program->size-1 < i)
		{
			printf("!!<warning> program ended \'halt\' never reached!!\n");
			exit(0);
		}
	}

	TokenizeData_free(tokenizeData);

	free(debugData);
	Stack_free(stack);
	arraylist_destroy(program);
}

void interpretLine(arraylist* program, /*out*/uint32_t *i, /*out*/Stack* stack, map_int_t *labelTracker, map_int_t *lineNumberTracker, DebugData *debugData)
{
	const char* command = arraylist_get(program, *i);
	debugData->commandName = command;
	(*i)++;

	const int* value = map_get(lineNumberTracker, uint32_toString(*i));
	if(value != NULL)
		debugData->currentLine = *value;

	const TKasmCommand TkCommand = getCommand(command);

	switch (TkCommand)
	{
	case tkasm_push:
	{
		const char* rawType  = arraylist_get(program, *i);

		if(getTypeClass_FromStr(rawType) == tkasmClass_array)
			tk_pushArray(/*out*/stack, rawType, arraylist_get(program, *i+1), debugData);
		else
			tk_push(/*out*/stack, rawType, arraylist_get(program, *i+1), debugData);

		*i += 2;
	}
	break;

	case tkasm_pop:
		checkIfStackIsEmpty(stack, debugData);
		tk_pop(/*out*/stack, arraylist_get(program, *i), debugData);
		(*i)++;
		break;

	case tkasm_movPush:
		exit_NotImplemented(command, debugData);

		(*i)++;
		break;

	case tkasm_movPop:
		exit_NotImplemented(command, debugData);

		*i += 2;
		break;

	case tkasm_mov:
		exit_NotImplemented(command, debugData);

		*i += 3;
		break;

	case tkasm_free:
		exit_NotImplemented(command, debugData);

		(*i)++;
		break;

	case tkasm_add:
		checkIfStackIsEmpty(stack, debugData);
		tk_add(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_sub:
		checkIfStackIsEmpty(stack, debugData);
		tk_sub(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_mull:
		checkIfStackIsEmpty(stack, debugData);
		tk_mull(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_div:
		checkIfStackIsEmpty(stack, debugData);
		tk_div(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_shiftLeft:
		checkIfStackIsEmpty(stack, debugData);
		tk_shiftLeft(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_shiftRight:
		checkIfStackIsEmpty(stack, debugData);
		tk_shiftRight(/*out*/stack, arraylist_get(program, *i), arraylist_get(program, *i+1), debugData);
		*i += 2;
		break;

	case tkasm_print:
		tk_print(arraylist_get(program, *i), debugData);
		(*i)++;
		break;

	case tkasm_printPop:
	{
		const char* rawType  = arraylist_get(program, *i);

		if(getTypeClass_FromStr(rawType) == tkasmClass_array)
			tk_printPopArray(/*out*/stack, rawType, arraylist_get(program, ++(*i)), debugData);
		else
			tk_printPop(/*out*/stack, rawType, debugData);

		(*i)++;
	}
	break;

	case tkasm_printPeek:
	{
		const char* rawType  = arraylist_get(program, *i);

		if(getTypeClass_FromStr(rawType) == tkasmClass_array)
			tk_printPeekArray(/*out*/stack, rawType, arraylist_get(program, ++(*i)), debugData);
		else
			tk_printPeek(/*out*/stack, rawType, debugData);

		(*i)++;
	}
	break;

	case tkasm_read:
		tk_read(/*out*/stack, arraylist_get(program, *i), debugData);
		(*i)++;
		break;

	case tkasm_jump:
		tk_jump(/*out*/i, labelTracker, arraylist_get(program, *i));
		break;

	case tkasm_jumpEquals0:
		if (tk_isEquals0(stack, arraylist_get(program, *i), debugData))
		{
			tk_jump(/*out*/i, labelTracker, arraylist_get(program, *i+1));
			return;
		}
		*i += 2;
		break;

	case tkasm_jumpGreater0:
		if (tk_isGreater0(/*out*/stack, arraylist_get(program, *i), debugData))
		{
			tk_jump(/*out*/i, labelTracker, arraylist_get(program, *i+1));
			return;
		}

		*i += 2;
		break;

	case tkasm_jumpSmaller0:
		if (tk_isSmaller0(/*out*/stack, arraylist_get(program, *i), debugData))
		{
			tk_jump(/*out*/i, labelTracker, arraylist_get(program, *i+1));
			return;
		}

		*i += 2;
		break;

	default:
		break;
	}
}