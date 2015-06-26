#pragma once

#include <stdio.h>

/**
 * @defgroup buh buh
 * @brief common macros
 * @addtogroup buh
 * @{
 */

#define herror(msg)                             \
  do { perror(msg); return -1; } while (0)

#define rerror(msg)                             \
  do {                                          \
    fprintf(stderr, msg);                       \
    return -1;                                  \
  } while (0)
