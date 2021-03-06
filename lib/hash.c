#include <stdio.h>

#include "buh/vector.h"
#include "buh/hash.h"

vec_define(hash_entry_t, entry);
vec_define(hash_bucket_t, bucket);

void
hash_dump(hash_table_t *ht)
{
  size_t entry_i, bucket_i;
  hash_bucket_t *bu;
  hash_entry_t *en;

  array_foreach(bucket_i, bu, ht->bucket.items, ht->bucket.size) {

    vec_foreach(entry_i, en, *bu) {

      printf("bucket %ld, entry %ld; k: %s; v: %s\n",
               bucket_i, entry_i, en->key, (char*)en->value);
    }
  }
}

unsigned long
hash_str(const char *s)
{
  unsigned long h = 0;
  while (*s)
    h = (h << 5) - h + *s++;
  return h;
}

void
hash_put(hash_table_t *ht, const char *key, void *value)
{
  size_t old;
  unsigned long hash;
  hash_entry_t *en;
  hash_bucket_t *bu;

  if (ht->item_count * 1.5 >= ht->bucket.size) {
    old = ht->bucket.size;
    vec_bucket_resize(&ht->bucket, old ? old << 1 : 16);
    if (old)
      hash_rehash(ht, old);
    else /* new hashtable; zeroize the entire bucket */
      memset(ht->bucket.items, '\0', sizeof (hash_bucket_t) * ht->bucket.size);
  }

  hash = hash_str(key);
  bu = ht->bucket.items + hash % ht->bucket.size;

  en = vec_entry_push(bu);
  en->key = strdup(key);
  en->hash = hash;
  en->value = value;

  ht->item_count++;
}

void
hash_rehash(hash_table_t *ht, size_t old)
{
  size_t entry_i, bucket_i;
  unsigned long index;
  hash_bucket_t *bu;
  hash_entry_t *en, *end;

  /* zeroize the second half */
  memset(ht->bucket.items + old, '\0', sizeof (hash_bucket_t) * (ht->bucket.size - old));

  array_foreach(bucket_i, bu, ht->bucket.items, old) {

    vec_foreach(entry_i, en, *bu) {
      index = en->hash % ht->bucket.size;
      if (index != bucket_i) {
        end = vec_entry_push(ht->bucket.items + index);
        memcpy(end, en, sizeof (hash_entry_t));
        vec_entry_splice(bu, entry_i);
        en--, entry_i--;
      }
    }
  }
}

void *
hash_get(hash_table_t *ht, const char *key)
{
  unsigned long hash;
  hash_bucket_t *bu;
  hash_entry_t *en;

  if (!ht->bucket.size)
    return NULL;

  hash = hash_str(key);
  bu = ht->bucket.items + (hash % ht->bucket.size);

  vec_iforeach(en, *bu) {
    if (hash == en->hash && strcmp(key, en->key) == 0)
      return en->value;
  }

  return NULL;
}

int
hash_iter(hash_table_t *ht, const char *key, hash_iter_t *iter)
{
  if (!ht->bucket.size)
    return -1;

  iter->hash = hash_str(key);
  iter->bu = ht->bucket.items + (iter->hash % ht->bucket.size);
  iter->en = iter->bu->items;

  return 0;
}

void *
hash_next(hash_iter_t *iter)
{
  void *value;

  for (; iter->en < iter->bu->items + iter->bu->count; ++iter->en)
    if (iter->hash == iter->en->hash) {
      value = iter->en->value;
      ++iter->en;
      return value;
    }

  return NULL;
}

void
hash_remove(hash_table_t *ht, const char *key)
{
  unsigned long hash, index;
  hash_bucket_t *bu;
  hash_entry_t *en;

  if (!ht->bucket.size)
    return;

  hash = hash_str(key);
  bu = ht->bucket.items + (hash % ht->bucket.size);

  vec_foreach(index, en, *bu) {
    if (hash == en->hash && strcmp(key, en->key) == 0) {
      free(en->key);
      vec_entry_splice(bu, index);
      ht->item_count--;
      break;
    }
  }
}
