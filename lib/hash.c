#include <buh/vector.h>
#include <buh/hash.h>

vec_define(hash_entry_t, entry);
vec_define(hash_bucket_t, bucket);

unsigned long
hash_str(char *s)
{
  unsigned long h = 0;
  while (*s)
    h = (h << 5) - h + *s++;
  return h;
}

void
hash_put(hash_table_t *ht, char *key, void *value)
{
  size_t old;
  unsigned long hash, i;
  hash_entry_t *en;
  hash_bucket_t *bu;

  if (ht->item_count * 1.5 >= ht->bucket.size) {
    old = ht->bucket.size;
    vec_bucket_resize(&ht->bucket, old ? old << 1 : 32);
    if (old)
      hash_rehash(ht, old);
    else /* new hashtable; zeroize the entire bucket */
      memset(ht->bucket.items, '\0', sizeof (hash_bucket_t) * ht->bucket.size);
  }

  hash = hash_str(key);
  bu = ht->bucket.items + hash % ht->bucket.size;

  vec_foreach(i, en, *bu) {
    if (hash == en->hash && strcmp(key, en->key) == 0)
      goto end;
  }

  en = vec_entry_push(bu);
  en->key = key;
  en->hash = hash;

 end:
  en->value = value;
  ht->item_count++;
}

void
hash_rehash(hash_table_t *ht, size_t old)
{
  size_t entry_i, bucket_i;
  unsigned long index;
  hash_bucket_t *bu;
  hash_entry_t *en, *eni, *end;

  /* zeroize the second half */
  memset(ht->bucket.items + old, '\0', sizeof (hash_bucket_t) * old);

  array_foreach(bucket_i, bu, ht->bucket.items, old) {

    eni = bu->items;

    vec_foreach(entry_i, en, *bu) {
      index = en->hash % ht->bucket.size;
      if (index != bucket_i) {
        end = vec_entry_push(ht->bucket.items + index);
        memcpy(end, en, sizeof (hash_entry_t));
      }
      else if (en != eni)
        memcpy(eni++, en, sizeof (hash_entry_t));
    }

    bu->count = eni - bu->items;
  }
}

void *
hash_get(hash_table_t *ht, char *key)
{
  unsigned long hash, i;
  hash_bucket_t *bu;
  hash_entry_t *en;

  hash = hash_str(key);
  bu = ht->bucket.items + (hash % ht->bucket.size);

  vec_foreach(i, en, *bu) {
    if (hash == en->hash && strcmp(key, en->key) == 0)
      return en->value;
  }

  return NULL;
}

void
hash_remove(hash_table_t *ht, char *key)
{
  unsigned long hash, index;
  hash_bucket_t *bu;
  hash_entry_t *en;

  hash = hash_str(key);
  bu = ht->bucket.items + (hash % ht->bucket.size);

  vec_foreach(index, en, *bu) {
    if (hash == en->hash && strcmp(key, en->key) == 0) {
      vec_entry_splice(bu, index);
      break;
    }
  }
}
