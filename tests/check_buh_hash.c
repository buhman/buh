#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "check_buh.h"

static int data_size = 1000;
static char **keys;
static char **values;

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

START_TEST (buh_check_hash)
{
  hash_table_t ht = {0};
  char **ki, **vi, *v;

  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {

    hash_put(&ht, *ki, *vi);
  }

  for (ki = keys, vi = values;
       ki < keys + data_size;
       ++ki, ++vi) {

    v = hash_get(&ht, *ki);

    assert(strcmp(v, *vi) == 0);
  }
}
END_TEST

int
main(int argc, char **argv)
{
  Suite *s = suite_create("BUH");
  TCase *tc = tcase_create("buh");
  tcase_add_test(tc, buh_check_hash);
  suite_add_tcase(s, tc);

  init_data();

  BUH_RUN();

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
