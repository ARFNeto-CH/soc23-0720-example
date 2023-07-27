#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    size_t limit;             // capacity
    size_t size;              // actual size
    void** item;              // an array of pointers to items
    void* (*copy)(void*);     // copy
    void* (*destroy)(void*);  // destructor
    int (*show)(void*);       // print
} Container;                  // a collection

Container* ctn_create(
    size_t, void* (*copy)(void*), void* (*destroy)(void*),
    int (*show)(void*));
Container* ctn_destroy(Container*);
int        ctn_insert(void*, Container*);
int        ctn_show(Container*);
