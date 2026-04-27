/*
 * Embroidermodder 2 -- String
 * Copyright 2011-2026 The Embroidermodder Team
 */

/*
 * TODO: tests for this file.
 */

#include <stdlib.h>

#include "core.h"

/* NOTE: the const char* fed in here is assumed to be properly null-terminated.
 */
String *str_create(const char *value)
{
    String *s = malloc(sizeof(String));
    s->data = malloc(STR_CHUNK);
    s->memory = STR_CHUNK;
    s->length = 0;
    str_const(s, value);
    return s;
}

/* This resizes the allocated memory as it copies. */
void str_const(String *dest, const char *src)
{
    int i;
    for (i=0; src[i]; i++) {
        dest->data[i] = src[i];
        if (i+2 > dest->memory) {
            dest->memory += STR_CHUNK;
            dest->data = realloc(dest->data, dest->memory);
        }
    }
    dest->length = i;
    dest->data[i] = 0;
}

void str_copy(String *dest, String *src)
{
    str_const(dest, src->data);
}

void str_concat(String *dest, const char *src)
{
    int i;
    int start = dest->length;
    for (i=0; src[i]; i++) {
        dest->data[start+i] = src[i];
        if (i+2 > dest->memory) {
            dest->memory += STR_CHUNK;
            dest->data = realloc(dest->data, dest->memory);
        }
    }
    dest->data[dest->length] = 0;
}

int str_compare(String *s1, const char *s2)
{
    int i;
    for (i=0; i<s1->length && s2[i]; i++) {
        int diff = s1->data[i] - s2[i];
        if (diff != 0) {
            return diff;
        }
    }
    if ((s1->data[i] == 0) && (s2[i] == 0)) {
        return 0;
    }
    return s1->data[i];
}

int str_find(String *src, const char *key)
{
    int i, j;
    for (i=0; i<src->length; i++) {
        int flag = 0;
        for (j=0; key[j]; j++) {
            if (i+j > src->length) {
                flag = 2;
                break;
            }
            if (src->data[i+j] - key[j]) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            return i;
        }
        if (flag == 2) {
            break;
        }
    }
    return -1;
}

void str_replace(String *src, String *from, String *to)
{
    int found = str_find(src, from->data);
    while (found >= 0) {
        /* FIXME */
        found = str_find(src, from->data);
    }
}

void str_free(String *str)
{
    free(str->data);
    free(str);
}

/* Dynamic memory management for string arrays. */
StrArray *
strarray_create(void)
{
    int i;
    StrArray *arr = malloc(sizeof(StrArray));
    arr->count = 0;
    arr->memory = STRARRAY_CHUNK;
    arr->data = malloc(sizeof(String*) * arr->memory);
    for (i=0; i<arr->memory; i++) {
        arr->data[i] = str_create("");
    }
    return arr;
}

void
strarray_append(StrArray *arr, const char *s)
{
    if (arr->memory < arr->count + 2) {
        int i;
        arr->data = realloc(arr->data, sizeof(String*) * (arr->count + STRARRAY_CHUNK));
        for (i=0; i<STRARRAY_CHUNK; i++) {
            arr->data[arr->memory + i] = str_create("");
        }
        arr->memory += STRARRAY_CHUNK;
    }
    str_const(arr->data[arr->count], s);
    arr->count++;
}

void
strarray_empty(StrArray *arr)
{
    arr->count = 0;
}

void
strarray_copy(StrArray *dest, StrArray *src)
{
    int i;
    strarray_empty(dest);
    for (i=0; i<src->count; i++) {
        strarray_append(dest, src->data[i]->data);
    }
}

void
strarray_free(StrArray *arr)
{
    int i;
    for (i=0; i<arr->memory; i++) {
        str_free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

