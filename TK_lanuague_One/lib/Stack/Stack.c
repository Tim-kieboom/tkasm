//
// Created by tim_k on 04/06/2024.
//

#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *Stack_create(void *new_data)
{
    struct node *new_node;

    Stack *new_list = (Stack *)malloc(sizeof (Stack));
    *new_list = (struct node *)malloc(sizeof (struct node));

    new_node = *new_list;
    new_node->data = new_data;
    new_node->next = NULL;
    return new_list;
}

void Stack_free(Stack *list)
{
    struct node *curr = *list;
    struct node *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

void Stack_push(Stack *list, void *data)
{
    struct node *head;
    struct node *new_node;
    if (list == NULL || *list == NULL) {
        fprintf(stderr, "llist_add_inorder: list is null\n");
    }

    head = *list;

    // Head is empty node
    if (head->data == NULL)
        head->data = data;

    // Head is not empty, add new node to front
    else {
        new_node = malloc(sizeof (struct node));
        new_node->data = data;
        new_node->next = head;
        *list = new_node;
    }
}

bool Stack_isEmpty(Stack *list)
{
    if((*list)->data == NULL)
        return true;
    else
        return false;
}

void *Stack_pop(Stack *list)
{
    void *popped_data;
    struct node *head = *list;

    if (list == NULL || head->data == NULL)
        return NULL;

    popped_data = head->data;
    *list = head->next;

    free(head);

    return popped_data;
}

void** Stack_array(Stack *list) {
    arraylist *dynArray = arraylist_create();

    while((*list)->next != NULL)
    {
        arraylist_add(dynArray, Stack_pop(list));
    }
    void** arr = arraylist_get(dynArray, 0);
    free(dynArray);
    return arr;
}

void Stack_print(Stack *list, void (*print)(void *))
{
    struct node *curr = *list;
    while (curr != NULL) {
        print(curr->data);
        printf(" ");
        curr = curr->next;
    }
    putchar('\n');
}