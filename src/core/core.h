/*
 * Embroidermodder 2 -- C Core
 * Copyright 2011-2026 The Embroidermodder Team
 */

#ifndef EMBROIDERMODDER_CORE_H
#define EMBROIDERMODDER_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

#define SDSARRAY_CHUNK                100

typedef struct SDSArray_ {
    sds *data;
    int count;
    int memory;
} sdsarray;

sdsarray *sdsarray_create(void);
void sdsarray_append(sdsarray *a, const char *s);
void sdsarray_empty(sdsarray *arr);
void sdsarray_copy(sdsarray *dest, sdsarray *src);
void sdsarray_free(sdsarray *a);

void state_create(void);
void state_free(void);

#ifdef __cplusplus
}
#endif

#endif

