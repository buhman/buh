#include <stdlib.h>
#include <string.h>

#include <buh/vector.h>

void
vec_push(vector_t *v, void *ptr, size_t size)
{
  vector_item_t *item;

  if (v->index == v->size) {
    v->size = v->size ? v->size << 1 : 2;
    v->items = realloc(v->items, sizeof (vector_item_t) * v->size);
  }

  item = v->items + v->index++;
  item->size = size;
  item->buf = ptr;
}

void
vec_slide(vector_t *v, size_t index)
{
  v->index -= index;
  memmove(v->items, v->items + index, sizeof (vector_item_t) * v->index);
}

void
vec_flush(vector_t *v)
{
  size_t index;

  for (index = 0; index < v->size; ++index)
    free((v->items + index)->buf);
}
