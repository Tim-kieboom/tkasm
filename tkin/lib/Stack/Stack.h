//
// Created by tim_k on 06/06/2024.
//

#ifndef MEMSTACK_H
#define MEMSTACK_H
#pragma once

#include <stdint.h>

struct stack
{
    uint8_t *data;
    ssize_t top;
    size_t size;
};
typedef struct stack Stack;

Stack* Stack_new(uint64_t size);

void Stack_push(Stack* st, const uint8_t value);
uint8_t Stack_pop(Stack* st);

uint8_t Stack_peek(const Stack* st);
uint8_t Stack_peekAt(const Stack* st, uint32_t amountUnderTop);

uint64_t Stack_size(const Stack* st);

void Stack_free(Stack* st);

#endif //MEMSTACK_H
