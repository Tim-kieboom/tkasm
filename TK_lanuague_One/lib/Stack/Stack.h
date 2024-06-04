//
// Created by tim_k on 04/06/2024.
//

#ifndef STACK_H
#define STACK_H

struct node
{
    void *data;
    struct node *next;
};

typedef struct node * Stack;

/* llist_create: Create a linked list */
Stack *Stack_create(void *data);

/* llist_free: Free a linked list */
void Stack_free(Stack *list);

/* llist_push: Add to head of list */
void Stack_push(Stack *list, void *data);

/* llist_pop: remove and return head of linked list */
void *Stack_pop(Stack *list);

/* llist_print: print linked list */
void Stack_print(Stack *list, void (*print)(void *data));

#endif //STACK_H
