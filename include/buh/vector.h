#pragma once

typedef struct vector_item {
  void *buf;
  size_t size;
} vector_item_t;

typedef struct {
  size_t index, size;
  vector_item_t *items;
} vector_t;

void
vec_push(vector_t *v, void *ptr, size_t size);

void
vec_slide(vector_t *v, size_t index);

void
vec_flush(vector_t *v);
