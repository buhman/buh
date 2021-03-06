#pragma once

#include <stdint.h>

#include <sys/epoll.h>

#include "buh/vector.h"

/**
 * @defgroup event event
 * @brief epoll helpers
 * @addtogroup event
 * @{
 */

typedef struct event_handler event_handler;

typedef int (event_handler_fptr)(event_handler*);

typedef int (event_callback_fptr)(event_handler*, void *buf, size_t len);

typedef struct buf {
  void *ptr;
  size_t size;
} buf_t;

/* vec_buf_t */
vec_declare(buf_t, buf);

/** generic event handler structure */
typedef struct event_handler {
  struct epoll_event ev;
  /** epoll file descriptor */
  int efd;
  /** socket file descriptor */
  int sfd;
  /** read */
  struct {
    event_handler_fptr *handler;
    event_callback_fptr *recv;
    event_handler_fptr *close;
  } in;
  /** write */
  struct {
    vec_buf_t queue;
  } out;
  /** data pointer */
  void *ptr;
} event_handler;

int
buh_event_iter(int efd);

int
buh_event_add(int efd, int sfd, uint32_t events, event_handler **oeh);

void
buh_event_close(event_handler *eh);
