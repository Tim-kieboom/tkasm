#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "lib/fileReader/fileReader.h"
#include "Interpreter/Interpreter.h"
#include "lib/stringTools/stringTools.h"
#include "tokenizer/mainTokenizer.h"

void cmdTkin(const uint32_t *fileLineCounter, TokenizeData *tokenizeData, arraylist *history, Stack *memStack, DebugData *debugData)
{
    arraylist *tokenizeHistory = (history == NULL) ? arraylist_create() : history;

    uint32_t i = 0;
    uint32_t programLineCounter = (fileLineCounter != NULL) ? *fileLineCounter+1 : 0;
    while(1)
    {
        printf("tkin> ");
        char inputbuffer[100];
        fgets(inputbuffer, sizeof(inputbuffer), stdin);
        if(inputbuffer[0] == '\n')
            continue;

        arraylist* tokenizedLines = tokenizeLine(inputbuffer, &i, tokenizeHistory, tokenizeData);
        if(tokenizedLines->size == 0)
        {
            arraylist_destroy(tokenizedLines);
            continue;
        }

        if(STR_EQUALS(arraylist_get(tokenizedLines, 0), "halt"))
        {
            arraylist_destroy(tokenizedLines);
            break;
        }

        interpretLine(tokenizeHistory, &programLineCounter, memStack, tokenizeData->labelTracker, tokenizeData->lineNumberTracker, debugData);
        arraylist_destroy(tokenizedLines);
    }

    arraylist_destroy(tokenizeHistory);
    TokenizeData_free(tokenizeData);
    free(debugData);
}

void cmdInterpretedMode(arraylist *history, const uint32_t *fileLineCounter)
{
    TokenizeData *data = TokenizeData_new();

    Stack *memStack = Stack_new(10000000);
    DebugData *debugData = DebugData_new("null", -1);

    cmdTkin(fileLineCounter, data, history, memStack, debugData);
}

void cmdInterpredFileMode(const char* filePath)
{
    printf("\nfilePath: %s\n\n", filePath);
    arraylist* file = readFile(filePath);

    TokenizeData *tokenizeData = TokenizeData_new();

    Stack *stack = Stack_new(10000000);
    DebugData *debugData = DebugData_new("null", -1);

    uint32_t i = 0;
    arraylist* tokenizeHistory = interpretFile(&i, file, stack, tokenizeData, debugData);
    printf("\n");
    cmdTkin(&i, tokenizeData, tokenizeHistory, stack, debugData);
}

void interpredFileMode(const char* filePath)
{
    printf("\nfilePath: %s\n\n", filePath);
    arraylist* file = readFile(filePath);
    interpretFile_andExit(file);
    arraylist_destroy(file);
}

void debugFunc()
{
    interpredFileMode("main.tkasm");
}

int main(int argc, char const *argv[])
{
    /*debugFunc();*/

    printf("argc: %d\n", argc);
    switch (argc)
    {
        case 1:
            cmdInterpretedMode(NULL, NULL);
            break;
        case 2:
            cmdInterpredFileMode(argv[1]);
            break;
        case 3:
            if(STR_EQUALS(argv[2], "-b"))
                interpredFileMode(argv[1]);
            else
                printf("!!<error> %s unknown argument!!", argv[2]);
            break;

        default:
            printf("!!<error> %d to many arguments!!", argc);
            exit(1);
    }

    return 0;
}
