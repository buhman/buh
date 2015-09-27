#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "check_buh.h"

static int data_size = 1000;
static char **keys;
static char **values;
static hash_table_t ht = {0};

static void
init_data(void)
{
  char buf[256], **ki, **vi;

  keys = calloc(data_size, sizeof(char*));
  values = calloc(data_size, sizeof(char*));

  srand(0);

  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {

    sprintf(buf, "%x", rand());
    *ki = strdup(buf);
    sprintf(buf, "%x", rand());
    *vi = strdup(buf);
  }
}

static void
buh_build_hash(void)
{
  char **ki, **vi, *v;

  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {

    hash_put(&ht, *ki, *vi);
  }

  hash_dump(&ht);

  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {

    v = hash_get(&ht, *ki);

    assert(strcmp(v, *vi) == 0);
  }
}

static void
buh_remove_hash(void)
{
  char **ki;

  for (ki = keys; ki < keys + data_size; ++ki) {

    assert(hash_get(&ht, *ki) != NULL);

    hash_remove(&ht, *ki);

    assert(hash_get(&ht, *ki) == NULL);
  }
}

int
main(int argc, char **argv)
{
  /*
  Suite *s = suite_create("BUH");
  TCase *tc = tcase_create("buh");
  tcase_add_test(tc, buh_build_hash);
  tcase_add_test(tc, buh_remove_hash);
  suite_add_tcase(s, tc);
  */

  init_data();

  buh_build_hash();
  buh_remove_hash();

  /*
  BUH_RUN();
  */

  char **ki, **vi;
  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {
    free(*ki);
    free(*vi);
  }

  free(keys);
  free(values);
}
