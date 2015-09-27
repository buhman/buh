#include "buh/hash.h"

#define BUH_RUN()                               \
  do {                                          \
    SRunner *sr;                                \
    int failed;                                 \
    sr = srunner_create(s);                     \
    srunner_run_all(sr, CK_NORMAL);             \
    failed = srunner_ntests_failed(sr);         \
    srunner_free(sr);                           \
    if (failed)                                 \
      return EXIT_FAILURE;                      \
    else                                        \
      return EXIT_SUCCESS;                      \
  } while (0);
