/*! @file emb-hash.h */
#ifndef EMB_HASH_H
#define EMB_HASH_H

/* This file contains wrapper functions around Keith Pomakis' HashTable Library */
#include "hashtable.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef HashTable EmbHash;

extern EMB_PUBLIC EmbHash* EMB_CALL embHash_create(void);
extern EMB_PUBLIC void EMB_CALL embHash_free(EmbHash* hash);

extern EMB_PUBLIC int EMB_CALL embHash_contains(const EmbHash* hash, const void* key);
extern EMB_PUBLIC int EMB_CALL embHash_insert(EmbHash* hash, const void* key, void* value);
extern EMB_PUBLIC void* EMB_CALL embHash_value(const EmbHash* hash, const void* key);
extern EMB_PUBLIC void EMB_CALL embHash_remove(EmbHash* hash, const void *key);
extern EMB_PUBLIC void EMB_CALL embHash_clear(EmbHash* hash);
extern EMB_PUBLIC int EMB_CALL embHash_empty(const EmbHash* hash);
extern EMB_PUBLIC long EMB_CALL embHash_count(const EmbHash* hash);
extern EMB_PUBLIC void EMB_CALL embHash_rehash(EmbHash* hash, long numOfBuckets);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_HASH_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
