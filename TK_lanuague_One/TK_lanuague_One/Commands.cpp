#include "Commands.h"
#include "stringTools.h"

TKasmCommand getCommand(const char* command)
{
    if (STR_EQUALS(command, "push"))
    {
        return tkasm_push;
    }
    else if (STR_EQUALS(command, "pop"))
    {
        return tkasm_pop;
    }

    else if (STR_EQUALS(command, "mov.push"))
    {
        return tkasm_movPush;
    }
    else if (STR_EQUALS(command, "mov.pop"))
    {
        return tkasm_movPop;
    }
    else if (STR_EQUALS(command, "mov"))
    {
        return tkasm_mov;
    }

    else if (STR_EQUALS(command, "free"))
    {
        return tkasm_free;
    }

    else if (STR_EQUALS(command, "add"))
    {
        return tkasm_add;
    }
    else if (STR_EQUALS(command, "sub"))
    {
        return tkasm_sub;
    }

    else if (STR_EQUALS(command, "print"))
    {
        return tkasm_print;
    }
    else if (STR_EQUALS(command, "print.pop"))
    {
        return tkasm_printPop;
    }

    else if (STR_EQUALS(command, "read"))
    {
        return tkasm_read;
    }

    else if (STR_EQUALS(command, "jump"))
    {
        return tkasm_jump;
    }
    else if (STR_EQUALS(command, "jump.equals.0"))
    {
        return tkasm_jumpEquals0;
    }
    else if (STR_EQUALS(command, "jump.greater.0"))
    {
        return tkasm_jumpGreater0;
    }

    else if (STR_EQUALS(command, "halt"))
    {
        return tkasm_halt;
    }

    cout << "!!<error> type not recognized command: " << command << "!!" << endl;
    exit(1);
}

void read(/*out*/Stack* stack, string& rawType, DebugData *data)
{
    TkasmType type = getType(rawType);
    void* value = readTypeFromConsole(type);

    if (value == nullptr)
        exit_TypeIsNotValid(rawType, data);

    uint8_t* segments = segmentType(type, value);
    pushType(type, segments, stack);

    delete[] segments;
}

void printPop(/*out*/Stack* stack, string& rawType, DebugData* data)
{
    TkasmType type = getType(rawType);

    uint8_t* segments = popType(type, stack);
    void* value = unsegmentType(type, segments);
    printTypeToConsole(type, value);
}