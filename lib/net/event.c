#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <buh/buh.h>
#include <buh/vector.h>
#include <buh/net/event.h>
#include <buh/net/handler.h>

vec_define(buf_t, buf);

/**
 * @addtogroup event
 * @{
 */

/**
 * Create new event for socket
 * @param efd epoll file descriptor
 * @param sfd socket file descriptor
 * @param events epoll events
 * @param oeh return event handler
 * @returns nonzero on failure
 */
int
buh_event_add(int efd, int sfd, uint32_t events,
              event_handler **oeh)
{
  int ret;
  event_handler *eh;

  eh = calloc(1, sizeof(event_handler));
  eh->efd = efd;
  eh->sfd = sfd;

  eh->ev.events = events;
  eh->ev.data.ptr = eh;

  ret = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &eh->ev);
  if (ret < 0) {
    free(eh);
    perror("epoll_ctl");
  }

  if (oeh)
    *oeh = eh;

  return 0;
}

/**
 * iterate through pending events, calling the event handler
 * @param efd epoll file descriptor
 * @returns nonzero on failure
 */
int
buh_event_iter(int efd) {
  int ret, nfds;
  struct epoll_event evs[64] = {0}, *evi;
  event_handler *eh;

  nfds = epoll_wait(efd, evs, 64, -1);
  if (nfds < 0)
    herror("epoll_wait");

  for (evi = evs; evi < evs + nfds; evi++) {

    eh = evi->data.ptr;

    if (evi->events & EPOLLIN) {

      if (eh->in.handler)
        ret = eh->in.handler(eh);
      else
        ret = buh_net_recv(eh);
      if (ret < 0)
        herror("buh_net_recv");
    }
    if (evi->events & EPOLLOUT) {

      ret = buh_net_send(eh);
      if (ret < 0)
        herror("buh_net_send");
    }
  }

  return 0;
}

void
buh_event_close(event_handler *eh)
{
  /*vec_flush(&eh->out.queue);*/
  close(eh->sfd);
  free(eh);
}
