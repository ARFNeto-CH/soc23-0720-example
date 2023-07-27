#pragma once
#include "container.h"

Container* ctn_create(
    size_t size, void* (*copy)(void*),
    void* (*destroy)(void*), int (*show)(void*))
{
    Container* ctn =
        (Container*)malloc(sizeof(Container) * size);
    if (ctn == NULL) return NULL;
    ctn->limit = size;  // limit
    ctn->size  = 0;     // now empty, of course
    ctn->item = (void**)malloc(size * sizeof(void*));
    if (ctn->item == NULL)
    {  // could not allocate
        free(ctn);
        return NULL;
    }
    ctn->copy    = copy;
    ctn->destroy = destroy;
    ctn->show    = show;
    return ctn;
}

Container* ctn_destroy(Container* ctn)
{  // to destroy a container we need to know how to
    // delete the voids: they can allocate memory
    if (ctn == NULL) return NULL;
    for (size_t ix = 0; ix < ctn->size; ix += 1)
        ctn->destroy(ctn->item[ix]);
    return NULL;
}

int ctn_insert(void* item, Container* ctn)
{  // it is not wise to insert just a pointer
    // that can be free'd elsewhere:
    // it must be a copy. But an void can allocate
    // memory so we need to rely on user supplied
    // method
    if (item == NULL) return -1;       // no void
    if (ctn == NULL) return -2;        // no container
    if (ctn->size == ctn->limit)
        return -3;                     // container full
    if (ctn->copy == NULL) return -4;  // no copy function
    ctn->item[ctn->size] = ctn->copy(item);
    if (ctn->item[ctn->size] == NULL)
        return -5;  // error on copy
    ctn->size += 1;
    return 0;
}

int ctn_show(Container* col)
{
    if (col == NULL) return -1;
    printf(
        "    there are %llu of %llu items\n\n", col->size,
        col->limit);
    if (col->show == NULL) return -1;  // no show function
    for (size_t i = 0; i < col->size; i += 1)
        col->show(col->item[i]);
    printf("[end of listing]\n");
    return 0;
}
