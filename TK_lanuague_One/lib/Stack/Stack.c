//
// Created by tim_k on 04/06/2024.
//

#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>

#include "../../metaData/Types.h"

Stack *Stack_create(void *data)
{
    Stack *new_list = malloc(sizeof (Stack));
    *new_list = (struct node *)malloc(sizeof (struct node));

    struct node *new_node = *new_list;
    new_node->data = data;
    new_node->next = NULL;
    return new_list;
}

void Stack_free(Stack *list)
{
    struct node *curr = *list;

    while (curr != NULL)
    {
        struct node *next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

void Stack_push(Stack *list, void *data)
{
    if (list == NULL || *list == NULL)
    {
        fprintf(stderr, "Stack_push: list is null\n");
    }

    struct node *head = *list;

    if(head == NULL)
    {
        fprintf(stderr, "!!<error> stack_push has no head!!\n");
        exit(1);
    }

    // Head is empty node
    if (head->data == NULL)
    {
        head->data = data;
    }
    else
    {
        struct node *new_node = malloc(sizeof(struct node));
        new_node->data = data;
        new_node->next = head;
        *list = new_node;
    }
}

bool Stack_isEmpty(const Stack *list)
{
    if((*list)->data == NULL)
        return true;
    else
        return false;
}

int64_t Stack_find(const Stack *list, void* value)
{
    struct node *curr = *list;
    uint32_t index = 0;

    while (curr != NULL)
    {
        if (*(uint64_t*)&curr->data == *(uint64_t*)&value)
            return index;

        curr = curr->next;
        index++;
    }
    return -1;
}

uint32_t Stack_size(const Stack *list)
{
    uint32_t index = 0;
    struct node *curr = *list;
    while (curr != NULL)
    {
        index++;
        curr = curr->next;
    }
    return index;

}

void *Stack_pop(Stack *list)
{
    if(list == NULL)
        return NULL;

    struct node *head = *list;

    if (*list == NULL || head->data == NULL)
        return NULL;

    void *popped_data = head->data;

    if(head->next == NULL)
    {
        head->data = NULL;
        return popped_data;
    }
    *list = head->next;

    free(head);

    return popped_data;
}

void** Stack_array(Stack *list)
{
    arraylist *dynArray = arraylist_create();

    while((*list)->next != NULL)
    {
        arraylist_add(dynArray, Stack_pop(list));
    }
    void** arr = arraylist_get(dynArray, 0);
    free(dynArray);
    return arr;
}

void Stack_print(const Stack *list, void (*print)(void *))
{
    const struct node *curr = *list;
    while (curr != NULL) {
        print(curr->data);
        printf(" ");
        curr = curr->next;
    }
    putchar('\n');
}