typedef HashTable EmbHash;

EMB_PUBLIC EmbHash* embHash_create(void);
EMB_PUBLIC void embHash_free(EmbHash* hash);

EMB_PUBLIC int embHash_contains(const EmbHash* hash, const void* key);
EMB_PUBLIC int embHash_insert(EmbHash* hash, const void* key, void* value);
EMB_PUBLIC void* embHash_value(const EmbHash* hash, const void* key);
EMB_PUBLIC void embHash_remove(EmbHash* hash, const void *key);
EMB_PUBLIC void embHash_clear(EmbHash* hash);
EMB_PUBLIC int embHash_empty(const EmbHash* hash);
EMB_PUBLIC long embHash_count(const EmbHash* hash);
EMB_PUBLIC void embHash_rehash(EmbHash* hash, long numOfBuckets);
