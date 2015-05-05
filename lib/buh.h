#pragma once

#include <stdio.h>

#define herror(msg)                             \
  do { perror(msg); return -1; } while (0)
