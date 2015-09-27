#pragma once

#include <string.h>
#include <stdlib.h>

#define vec_declare(T, name)                                    \
                                                                \
  typedef struct {                                              \
    T *items;                                                   \
    size_t count, size;                                         \
  } vec_##name##_t;                                             \
                                                                \
  T*                                                            \
  vec_##name##_push(vec_##name##_t *vec);                       \
                                                                \
  void                                                          \
  vec_##name##_slide(vec_##name##_t *vec, size_t index);        \
                                                                \
  void                                                          \
  vec_##name##_resize(vec_##name##_t *vec, size_t size);        \
                                                                \
  void                                                          \
  vec_##name##_splice(vec_##name##_t *vec, size_t index);

#define vec_define(T, name)                                             \
                                                                        \
  inline T*                                                             \
  vec_##name##_push(vec_##name##_t *vec)                                \
  {                                                                     \
    if (vec->count == vec->size)                                        \
      vec_##name##_resize(vec, vec->size ? vec->size << 1 : 2);         \
                                                                        \
    return vec->items + vec->count++;                                   \
  }                                                                     \
                                                                        \
  inline void                                                           \
  vec_##name##_slide(vec_##name##_t *vec, size_t index)                 \
  {                                                                     \
    vec->count -= index;                                                \
    memmove(vec->items, vec->items + index, sizeof (T) * vec->count);   \
  }                                                                     \
                                                                        \
  inline void                                                           \
  vec_##name##_resize(vec_##name##_t *vec, size_t size)                 \
  {                                                                     \
    vec->size = size;                                                   \
    vec->items = realloc(vec->items, sizeof (T) * vec->size);           \
  }                                                                     \
                                                                        \
  inline void                                                           \
  vec_##name##_splice(vec_##name##_t *vec, size_t index)                \
  {                                                                     \
    if (index + 1 < vec->count) {                                       \
      memmove(vec->items + index, vec->items + index + 1,               \
              sizeof (T) * (vec->count - index));                       \
    }                                                                   \
    vec->count--;                                                       \
  }                                                                     \
                                                                        \
  inline void                                                           \
  vec_##name##_free(vec_##name##_t *vec)                                \
  {                                                                     \
    if (vec->items)                                                     \
      free(vec->items);                                                 \
  }


#define array_foreach(index, iter, array, items)                \
  for (index = 0, iter = array; index < items; ++index, ++iter)

#define vec_foreach(index, iter, vec)           \
  for (index = 0, iter = (vec).items;           \
       iter < (vec).items + (vec).count;        \
       ++index, ++iter)

#define vec_iforeach(iter, vec)                 \
  for (iter = (vec).items;                      \
       iter < (vec).items + (vec).count;        \
       ++iter)
