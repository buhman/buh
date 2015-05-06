#pragma once

#include <stdint.h>

/**
 * @defgroup event event
 * @brief epoll helpers
 * @addtogroup event
 * @{
 */

typedef struct event_handler event_handler;

typedef int (event_handler_fptr)(event_handler*);

/** generic event handler structure */
typedef struct event_handler {
  /** epoll file descriptor */
  int efd;
  /** socket file descriptor */
  int sfd;
  /** read handler function */
  event_handler_fptr *rh;
  /** write handler function */
  event_handler_fptr *wh;
  /** data pointer */
  void *ptr;
} event_handler;

int
buh_event_iter(int efd);

int
buh_event_add(int efd, int sfd, uint32_t events, event_handler **oeh);
