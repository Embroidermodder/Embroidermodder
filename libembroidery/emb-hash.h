#ifndef EMB_HASH_H
#define EMB_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* This file contains wrapper functions around Keith Pomakis' HashTable Library */

#include "hashtable.h"
typedef HashTable EmbHash;

EmbHash* embHash_create();
void embHash_free(EmbHash* hash);

int embHash_contains(const EmbHash* hash, const void* key);
int embHash_insert(EmbHash* hash, const void* key, void* value);
void* embHash_value(const EmbHash* hash, const void* key);
void embHash_remove(EmbHash* hash, const void *key);
void embHash_clear(EmbHash* hash);
int embHash_empty(const EmbHash* hash);
long embHash_count(const EmbHash* hash);
void embHash_rehash(EmbHash* hash, long numOfBuckets);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_HASH_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
