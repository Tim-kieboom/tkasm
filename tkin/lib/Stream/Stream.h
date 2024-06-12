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

typedef struct node * Stream;

Stream *Stream_create(void *data);

void Stream_free(Stream *list);

void Stream_push(Stream *list, void *data);

void *Stream_pop(Stream *list);

//only works if value is <= 64bits
int64_t Stream_find(const Stream *list, void* value);

uint32_t Stream_size(const Stream *list);

void** Stream_array(Stream *list);

bool Stream_isEmpty(const Stream *list);

void Stream_print(const Stream *list, void (*print)(void *data));

#endif //STACK_H
