//
// Created by tim_k on 06/06/2024.
//

#include "Stack.h"

#include <assert.h>
#include <stdlib.h>

Stack* Stack_new(const size_t size)
{
    Stack* st = malloc(sizeof(Stack));
    st->data = malloc(size * sizeof(uint8_t));
    st->top = -1;
    st->size = size;
    return st;
}


void Stack_push(Stack* st, const uint8_t value)
{
    assert(st->top+2 <= st->size);
    st->data[++st->top] = value;
}

uint8_t Stack_pop(Stack* st)
{
    assert(st->top >= 0);
    return st->data[st->top--];
}

uint8_t Stack_peek(const Stack* st)
{
    assert(st->top >= 0);
    return st->data[st->top];
}

uint8_t Stack_peekAt(const Stack* st, const uint32_t amountUnderTop)
{
    assert(st->top-amountUnderTop >= 0);
    return st->data[st->top-amountUnderTop];
}

uint64_t Stack_size(const Stack* st)
{
    return st->top + 1;
}


void MemStack_free(Stack* st)
{
    if(st->data != NULL)
        free(st->data);

    free(st);
}