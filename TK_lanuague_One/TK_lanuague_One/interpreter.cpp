#include "interpreter.h"
#include <list>
#include "Types.h"
#include "Commands.h"
#include "debug.h"


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

	print "value":                      print string_literal to terminal
	print.pop /%type/:                  print the top of the stack

	read %type:                         read number from io input and push it to stack

	jump (label):                       jump to label
	jump.equals.0 /%type/ (label):      jump to label if top of stack is 0
	jump.greater.0 /%type/ (label):     jump to label if top of stack is greater then 0
	jump.smaller.0 /%type/ (label):     jump to label if top of stack is smaller then 0

	halt:                               stop program

*/

void exit_TypeNotFound(string rawType, DebugData* data)
{
	cout << "!!<error> type[" << rawType << "] not a valid type" << stringOfDebugData(data) << endl;
	exit(1);
}

void exit_NotImplemented(string command, DebugData* data)
{
	cout << "!!<error> command[" << command << "] not implemented" << stringOfDebugData(data) << endl;
	exit(1);
}

void printTokenized(vector<string>* lines, vector<string>& program, unordered_map < string, uint32_t>& labelTracker)
{
	cout << "raw tsamk =====================================" << endl << endl;
	for (size_t i = 0; i < lines->size(); i++)
	{
		cout << (i + 1) << ".\t" << lines->at(i) << endl;
	}
	cout << endl;

	cout << "tokenized =====================================" << endl << endl;
	for (size_t i = 0; i < program.size(); i++)
	{
		cout << i << ".\t" << program.at(i) << endl;
	}

	cout << endl << "label tracker =====================================" << endl;
	cout << '[';
	for (pair<const string, uint32_t>& pair : labelTracker)
	{
		cout << "<(\"" << pair.first << "\", " << pair.second << ")>, ";
	}
	cout << ']' << endl;
	cout << endl << "=====================================" << endl;
}

void interpretCode(vector<string>* lines)
{
	auto labelTracker = unordered_map<string, uint32_t>();
	auto lineNumberTracker = unordered_map<uint32_t, uint32_t>();
	vector<string> program = tokenizer(lines, /*out*/labelTracker, /*out*/lineNumberTracker);

	auto stack = new Stack();
	auto debugData = new DebugData();

	printTokenized(lines, program, labelTracker);

	uint32_t i = 0;
	while (strcmp(program[i].data(), "halt") != 0)
	{
		const char* command = program[i].data();
		debugData->commandName = command;
		i++;

		debugData->currentLine = lineNumberTracker[i];

		TKasmCommand TkCommand = getCommand(command);

		switch (TkCommand)
		{
		case tkasm_push:
			push(/*out*/stack, program[i], program[i + 1], debugData);
			i += 2;
			break;

		case tkasm_pop:
			checkIfStackIsEmpty(stack, debugData);
			pop(/*out*/stack, program[i], debugData);
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
			add(/*out*/stack, program[i], program[i + 1], debugData);
			i += 2;
			break;

		case tkasm_sub:
			checkIfStackIsEmpty(stack, debugData);
			sub(/*out*/stack, program[i], program[i + 1], debugData);
			i += 2;
			break;

		case tkasm_mull:
			checkIfStackIsEmpty(stack, debugData);
			mull(/*out*/stack, program[i], program[i + 1], debugData);
			i += 2;
			break;

		case tkasm_div:
			checkIfStackIsEmpty(stack, debugData);
			div(/*out*/stack, program[i], program[i + 1], debugData);
			i += 2;
			break;

		case tkasm_print:
			print(program[i], debugData);
			i++;
			break;

		case tkasm_printPop:
			printPop(/*out*/stack, program[i], debugData);
			i++;
			break;

		case tkasm_read:
			read(/*out*/stack, program[i], debugData);
			i++;
			break;

		case tkasm_jump:
			jump(/*out*/i, labelTracker, program[i]);
			break;

		case tkasm_jumpEquals0:
			if (isEquals0(stack, program[i], debugData))
			{
				jump(/*out*/i, labelTracker, program[i + 1]);
				continue;
			}
			i += 2;
			break;

		case tkasm_jumpGreater0:
			if (isGreater0(/*out*/stack, program[i], debugData))
			{
				jump(/*out*/i, labelTracker, program[i + 1]);
				continue;
			}

			i += 2;
			break;

		case tkasm_jumpSmaller0:
			if (isSmaller0(/*out*/stack, program[i], debugData))
			{
				jump(/*out*/i, labelTracker, program[i + 1]);
				continue;
			}

			i += 2;
			break;

		default:
			break;
		}
	}
	delete debugData;
	delete stack;
}