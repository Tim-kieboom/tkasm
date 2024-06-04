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

/* Stack_create: Create a linked list */
Stack *Stack_create(void *data);

/* Stack_free: Free a linked list */
void Stack_free(Stack *list);

/* Stack_push: Add to head of list */
void Stack_push(Stack *list, void *data);

/* Stack_pop: remove and return head of linked list */
void *Stack_pop(Stack *list);

/* Stack_array: return array of linkedlist */
void** Stack_array(Stack *list);

bool Stack_isEmpty(Stack *list);

/* Stack_print: print linked list */
void Stack_print(Stack *list, void (*print)(void *data));

#endif //STACK_H
