/*
 * Embroidermodder 2 -- sds arrays
 * Copyright 2011-2026 The Embroidermodder Team
 */

#include <stdlib.h>

#include "core.h"

/* Dynamic memory management for string arrays. */
sdsarray *
sdsarray_create(void)
{
    int i;
    sdsarray *arr = malloc(sizeof(sdsarray));
    arr->count = 0;
    arr->memory = SDSARRAY_CHUNK;
    arr->data = malloc(sizeof(sds) * arr->memory);
    for (i=0; i<arr->memory; i++) {
        arr->data[i] = sdsempty();
    }
    return arr;
}

void
sdsarray_append(sdsarray *arr, const char *s)
{
    if (arr->memory < arr->count + 2) {
        int i;
        arr->data = realloc(arr->data, sizeof(sds) * (arr->count + SDSARRAY_CHUNK));
        for (i=0; i<SDSARRAY_CHUNK; i++) {
            arr->data[arr->memory + i] = sdsempty();
        }
        arr->memory += SDSARRAY_CHUNK;
    }
    sdsclear(arr->data[arr->count]);
    arr->data[arr->count] = sdscat(arr->data[arr->count], s);
    arr->count++;
}

void
sdsarray_empty(sdsarray *arr)
{
    arr->count = 0;
}

void
sdsarray_copy(sdsarray *dest, sdsarray *src)
{
    int i;
    sdsarray_empty(dest);
    for (i=0; i<src->count; i++) {
        sdsarray_append(dest, src->data[i]);
    }
}

void
sdsarray_free(sdsarray *arr)
{
    int i;
    for (i=0; i<arr->count; i++) {
        sdsfree(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

