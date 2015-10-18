#include "buh/vector.h"

typedef struct hash_entry {
  unsigned long hash;
  char *key;
  void *value;
} hash_entry_t;

/* vec_entry_t */
vec_declare(hash_entry_t, entry);

typedef vec_entry_t hash_bucket_t;

/* vec_bucket_t */
vec_declare(hash_bucket_t, bucket);

typedef struct hash_table {
  vec_bucket_t bucket;
  size_t item_count; /* total item count */
} hash_table_t;

void
hash_dump(hash_table_t *ht);

unsigned long
hash_str(const char *s);

void
hash_put(hash_table_t *ht, const char *key, void *value);

void
hash_rehash(hash_table_t *ht, size_t old);

void *
hash_get(hash_table_t *ht, const char *key);

void
hash_remove(hash_table_t *ht, const char *key);

typedef struct hash_iter {
  unsigned long hash;
  hash_bucket_t *bu;
  hash_entry_t *en;
} hash_iter_t;

int
hash_iter(hash_table_t *ht, const char *key, hash_iter_t *iter);

void *
hash_next(hash_iter_t *iter);
