#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

typedef struct
{
    size_t limit;             // capacity
    size_t size;              // actual size
    Item** item;              // an array of pointers to items
    Item* (*copy)(Item*);     // copy
    Item* (*destroy)(Item*);  // destructor
    int (*show)(Item*);       // print
} Container;                  // a collection

Container* ctn_create(
    size_t, Item* (*copy)(Item*), Item* (*destroy)(Item*),
    int (*show)(Item*));
Container* ctn_destroy(Container*);
int        ctn_insert(Item*, Container*);
int        ctn_show(Container*);
