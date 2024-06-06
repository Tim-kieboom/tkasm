//
// Created by tim_k on 04/06/2024.
//

#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stdint.h>
#include "../ArrayList/ArrayList.h"

struct node
{
    void *data;
    struct node *next;
};

typedef struct node * Stack;

Stack *Stack_create(void *data);

void Stack_free(Stack *list);

void Stack_push(Stack *list, void *data);

void *Stack_pop(Stack *list);

//only works if value is <= 64bits
int64_t Stack_find(const Stack *list, void* value);

uint32_t Stack_size(const Stack *list);

void** Stack_array(Stack *list);

bool Stack_isEmpty(const Stack *list);

void Stack_print(const Stack *list, void (*print)(void *data));

#endif //STACK_H
