#pragma once

#include <stdint.h>

typedef struct event_handler event_handler;

typedef int (event_handler_fptr)(event_handler*);

typedef struct event_handler {
  int efd;
  int sfd;
  event_handler_fptr *rh;
  event_handler_fptr *wh;
  void *ptr;
} event_handler;

int
buh_event_add(int efd, int sfd, uint32_t events,
              event_handler **oeh);

int
buh_event_iter(int efd);
