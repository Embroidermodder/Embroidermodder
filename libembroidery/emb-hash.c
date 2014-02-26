#include "emb-hash.h"

/* This file contains wrapper functions around Keith Pomakis' HashTable Library */

EmbHash* embHash_create(void) { return HashTableCreate(1); }
void embHash_free(EmbHash* hash) { HashTableDestroy(hash); hash = 0; }

int embHash_contains(const EmbHash* hash, const void* key) { return HashTableContainsKey(hash, key); }
int embHash_insert(EmbHash* hash, const void* key, void* value) { return HashTablePut(hash, key, value); }
void* embHash_value(const EmbHash* hash, const void* key) { return HashTableGet(hash, key); }
void embHash_remove(EmbHash* hash, const void *key) { HashTableRemove(hash, key); }
void embHash_clear(EmbHash* hash) { HashTableRemoveAll(hash); }
int embHash_empty(const EmbHash* hash) { return HashTableIsEmpty(hash); }
long embHash_count(const EmbHash* hash) { return HashTableSize(hash); }
void embHash_rehash(EmbHash* hash, long numOfBuckets) { HashTableRehash(hash, numOfBuckets); }

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
