#include "interpreter.h"
#include "tokenizer.h"
#include <list>

#define JUMP_TO_LABEL(hashMap, label) hashMap[label] 

/*
instructions needed:
    push %type (value):           push stack
    pop %type:                    pop stack

    mov.pop %type (name):         move top of stack to heap
    mov %type (name) (value):     move value to heap

    free (name):                  free value to heap

    add %type:                    pop 2 numbers from the stack and sum push the sum
    sub %type:                    pop 2 numbers and push the subtraction

    print "value":                print string_literal to terminal
    print.pop %type:              print the top of the stack

    read %type:                   read number from io input and push it to stack

    jump (label)
    jump.equals.0 %type (label):  jump to label if top of stack is 0
    jump.greater.0 %type (label): jump to label if top of stack is greater then 0

    halt:                   stop program

*/

class Heap 
{
private:
    int lastAddress;
    int32_t* memory;
    unordered_map<string, int32_t> variableNames;
    stack<int32_t> freeSpaces;

public:
    Heap()
    {
        lastAddress = -1;
        memory = new int32_t[1'000'000];
        variableNames = unordered_map<string, int32_t>();
        freeSpaces = stack<int32_t>();
    }

    ~Heap()
    {
        if(memory != nullptr)
            delete memory;
    }

    bool insert(string name, int32_t value)
    {
        if(!freeSpaces.empty())
        {
            int32_t address = freeSpaces.top();
            freeSpaces.pop();

            insertName(name, address);
            memory[address] = value;
            return true;
        }

        lastAddress++;
        insertName(name, lastAddress);
        memory[lastAddress] = value;
        return true;
    }

    int32_t get(string name)
    {
        return memory[getAddress(name)];
    }

    bool free(string name)
    {
        int32_t memAddress = getAddress(name);
        if (memAddress == lastAddress)
        {
            lastAddress--;
            variableNames.erase(name);
            return true;
        }

        freeSpaces.push(memAddress);
        variableNames.erase(name);
        return true;
    }

    void print()
    {
        cout << "heap =========================================" << endl;
        for (auto i = variableNames.begin(); i != variableNames.end(); i++)
        {
            cout << i->first << ", " << memory[i->second] << endl;
        }
    }

private:
    void insertName(string name, int32_t address)
    {
        variableNames.insert({ name, address });
    }

    int32_t getAddress(string name)
    {
        return variableNames.at(name);
    }
};

void exitTypeNotFound(string rawType, DebugData* data)
{
    cout << "!!<error> type[" << rawType << "] not a valid type" << stringOfDebugData(data) << endl;
}

TkasmType getType(string rawType, DebugData *data)
{
    const char* type = rawType.data();
    if(STR_EQUALS(type, "%char"))
    {
        return tkasm_char;
    }
    else if(STR_EQUALS(type, "%int"))
    {
        return tkasm_int;
    }

    cout << "!!<error> type[" << rawType << "] not a valid type" << stringOfDebugData(data) << endl;
    exit(1);
}

template<typename T>
T getValueFromStack(TkasmType type, int32_t rawValue, string rawType, DebugData *data)
{
    T value;

    switch (type)
    {
    case tkasm_char:
        value = (char)rawValue;
        break;

    case tkasm_int:
        value = (int32_t)rawValue;
        break;

    default:
        exitTypeNotFound(rawType, data);
        break;
    }

    return value;
}

template<typename T>
T getInput()
{
    T input;
    cin >> input;
    return input;
}

void read(stack<int32_t> *memStack, string rawType, DebugData *data)
{
    TkasmType type = getType(rawType, data);
    int32_t input;
    
    switch (type)
    {
    case tkasm_char:
        input = (int32_t)getInput<char>();
        break;

    case tkasm_int:
        input = getInput<int32_t>();
        break;

    default:
        exitTypeNotFound(rawType, data);
        break;
    }
    memStack->push(input);
}

void push(stack<int32_t> *memStack, string rawType, string rawValue, DebugData *data)
{
    TkasmType type = getType(rawType, data);
    int32_t value;

    switch (type)
    {
    case tkasm_char:
        value = (int32_t)rawValue[0];
        break;

    case tkasm_int:
        value = stringTo_int32(rawValue.data());
        break;

    default:
        exitTypeNotFound(rawType, data);
        break;
    }
    memStack->push(value);
}


void genericAdd(stack<int32_t> *memStack, TkasmType type, int32_t first, int32_t second)
{
    switch (type)
    {
    case tkasm_char:
        memStack->push(first + second);
        break;

    case tkasm_int:
        memStack->push(first + second);
        break;

    default:
        break;
    }
}

void genericSub(stack<int32_t> *memStack, TkasmType type, int32_t first, int32_t second)
{
    switch (type)
    {
    case tkasm_char:
        memStack->push(second - first);
        break;

    case tkasm_int:
        memStack->push(second - first);
        break;

    default:
        break;
    }
}

void add(stack<int32_t>* memStack, string rawType, DebugData* data)
{
    TkasmType type = getType(rawType, data);

    checkIfStackIsEmpty(memStack, data);
    int32_t first = memStack->top();
    memStack->pop();

    checkIfStackIsEmpty(memStack, data);
    int32_t second = memStack->top();
    memStack->pop();

    genericAdd(memStack, type, first, second);
}

void sub(stack<int32_t>* memStack, string rawType, DebugData *data)
{
    TkasmType type = getType(rawType, data);

    checkIfStackIsEmpty(memStack, data);
    int32_t first = memStack->top();
    memStack->pop();

    checkIfStackIsEmpty(memStack, data);
    int32_t second = memStack->top();
    memStack->pop();

    genericSub(memStack, type, first, second);
}

void printPop(stack<int32_t>* memStack, string rawType, DebugData* data)
{
    TkasmType type = getType(rawType, data);

    checkIfStackIsEmpty(memStack, data);
    int32_t number = memStack->top();
    memStack->pop();

    switch (type)
    {
    case tkasm_char:
        cout << (char)number;
        break;

    case tkasm_int:
        cout << number;
        break;

    default:
        break;
    }

}

bool isEquals0(stack<int32_t> * memStack, DebugData *data)
{
    checkIfStackIsEmpty(memStack, data);
    int32_t number = memStack->top();

    return number == 0;
}

bool isGreater0(stack<int32_t>* memStack, DebugData* data)
{
    checkIfStackIsEmpty(memStack, data);
    int32_t number = memStack->top();

    return number > 0;
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
    auto memStack = new stack<int32_t>();

    auto heap = new Heap();

    auto labelTracker = unordered_map<string, int32_t>();
    auto lineNumberTracker = unordered_map<int32_t, int32_t>();
    vector<string> program = tokenizer(lines, /*out*/labelTracker, /*out*/lineNumberTracker);

    printTokenized(lines, program);

    size_t i = 0;
    auto debugData = new DebugData();

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
            push(memStack, program[i], program[i + 1], debugData);
            i += 2;
            break;

        case tkasm_pop:
            checkIfStackIsEmpty(memStack, debugData);
            memStack->pop();
            break;

        case tkasm_movPop:
        {
            checkIfStackIsEmpty(memStack, debugData);
            int32_t number = memStack->top();
            memStack->pop();

            heap->insert(program[i+1], number);

            i += 2;
        }
        break;

        case tkasm_mov:
        {
            TkasmType type = getType(program[i], debugData);

            string rawValue = program[i + 2];
            int32_t value = stringTo_int32(rawValue.data());
            
            heap->insert(program[i+1], value);
            i += 3;
        }
        break;

        case tkasm_free:
            heap->free(program[i]);
            i++;
            break;

        case tkasm_add:
            add(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_sub:
            sub(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_print:
            cout << program[i] << endl;
            i++;
            break;

        case tkasm_printPop:
            printPop(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_read:
            read(memStack, program[i], debugData);
            i++;
            break;

        case tkasm_jump:
            i = JUMP_TO_LABEL(labelTracker, program[i]);
            break;

        case tkasm_jumpEquals0:

            if (isEquals0(memStack, debugData))
            {
                i = JUMP_TO_LABEL(labelTracker, program[i+1]);
            }
            else
            {
                i += 2;
            }
            break;

        case tkasm_jumpGreater0:

            if (isGreater0(memStack, debugData))
            {
                i = JUMP_TO_LABEL(labelTracker, program[i+1]);
            }
            else
            {
                i += 2;
            }
            break;

        default:
            break;
        }
    }

    heap->print();

    delete debugData;
    delete memStack;
    delete heap;
}