#include "interpreter.h"
#include <list>
#include "Types.h"
#include "Commands.h"
#include "debug.h"

#define JUMP_TO_LABEL(hashMap, label) hashMap[label] 

/*
instructions needed:
    push %type (value):           push stack
    pop %type:                    pop stack

    mov.push (name):              move value in heap to stack
    mov.pop %type (name):         move top of stack to heap
    mov %type (name) (value):     move value to heap

    free (name):                  free value to heap

    add %type:                    pop 2 numbers from the stack and sum push the sum
    sub %type:                    pop 2 numbers and push the subtraction

    print "value":                print string_literal to terminal
    print.pop %type:              print the top of the stack

    read %type:                   read number from io input and push it to stack

    jump (label):                 jump to label
    jump.equals.0 %type (label):  jump to label if top of stack is 0
    jump.greater.0 %type (label): jump to label if top of stack is greater then 0

    halt:                   stop program

*/

void exit_TypeNotFound(string rawType, DebugData* data)
{
    cout << "!!<error> type[" << rawType << "] not a valid type" << stringOfDebugData(data) << endl;
    exit(1);
}

void exit_NotImplemented(string command, DebugData *data)
{
    cout << "!!<error> command[" << command << "] not implemented" << stringOfDebugData(data) << endl;
    exit(1);
}

void printTokenized(vector<string> *lines, vector<string> &program)
{
    cout << "raw tsamk =====================================" << endl << endl;
    for (size_t i = 0; i < lines->size(); i++)
    {
        cout << (i+1) << ".\t" << lines->at(i) << endl;
    }
    cout << endl;

    cout << "tokenized =====================================" << endl << endl;
    for (string line : program)
    {
        cout << line << endl;
    }
    cout << endl << "=====================================" << endl;
}

void interpretCode(vector<string>* lines)
{
    auto labelTracker = unordered_map<string, uint32_t>();
    auto lineNumberTracker = unordered_map<uint32_t, uint32_t>();
    vector<string> program = tokenizer(lines, /*out*/labelTracker, /*out*/lineNumberTracker);

    auto stack = new Stack();
    auto debugData = new DebugData();
    
    printTokenized(lines, program);

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
            exit_NotImplemented(command, debugData);

            //push(memStack, program[i], program[i + 1], debugData);
            i += 2;
            break;

        case tkasm_pop:
            exit_NotImplemented(command, debugData);

            checkIfStackIsEmpty(stack, debugData);
            stack->pop();
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
            exit_NotImplemented(command, debugData);

            //add(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_sub:
            exit_NotImplemented(command, debugData);

            //sub(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_print:
            cout << program[i] << endl;
            i++;
            break;

        case tkasm_printPop:
            printPop(stack, program[i], debugData);
            i++;
            break;

        case tkasm_read:
            read(stack, program[i], debugData);
            i++;
            break;

        case tkasm_jump:
            i = JUMP_TO_LABEL(labelTracker, program[i]);
            break;

        case tkasm_jumpEquals0:
            exit_NotImplemented(command, debugData);
            //if (isEquals0(stack, debugData))
            //{
            i = JUMP_TO_LABEL(labelTracker, program[i + 1]);
            //}
            //else
            {
                i += 2;
                //}
                break;

        case tkasm_jumpGreater0:
            exit_NotImplemented(command, debugData);
            //if (isGreater0(stack, debugData))
            //{
            i = JUMP_TO_LABEL(labelTracker, program[i + 1]);
            //}
            //else
            //{
            i += 2;
            //}
            break;

        default:
            break;
            }
        }
    }
    delete debugData;
    delete stack;
}