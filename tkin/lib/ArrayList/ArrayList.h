/* This file was automatically generated.  Do not edit! */
#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct arraylist arraylist;
void arraylist_destroy(arraylist *l);
void arraylist_splice(arraylist *l,arraylist *source,unsigned int index);
void arraylist_join(arraylist *l,arraylist *source);
arraylist *arraylist_copy(arraylist *l);
arraylist *arraylist_slice_end(arraylist *l,unsigned int index);
arraylist *arraylist_slice(arraylist *l,unsigned int index,unsigned int length);
void arraylist_clear(arraylist *l);
void *arraylist_remove(arraylist *l,unsigned int index);
void arraylist_insert(arraylist *l,unsigned int index,void *value);
void arraylist_set(arraylist *l,unsigned int index,void *value);
void *arraylist_get(arraylist *l,unsigned int index);
void *arraylist_pop(arraylist *l);
void arraylist_add(arraylist *l,void *item);
void arraylist_addArray(arraylist* l, void** array, const size_t sizeArray);
void arraylist_allocate(arraylist *l,unsigned int size);
arraylist *arraylist_create();
void** arraylist_CopyToArray(const arraylist* l, const size_t sizeValue);
#define arraylist_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])
struct arraylist {
	int64_t size; // Count of items currently in list
	int64_t capacity; // Allocated memory size, in items
	void** body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};
#define INTERFACE 0