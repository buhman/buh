#include <buh/vector.h>

typedef struct hash_entry {
  unsigned long hash;
  const char *key;
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
