#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>

#include "buh.h"
#include "event.h"

int
buh_event_add(int efd, int sfd, uint32_t events,
              event_handler **oeh)
{
  int ret;
  event_handler *eh;
  struct epoll_event ev;

  eh = calloc(1, sizeof(event_handler));
  eh->efd = efd;
  eh->sfd = sfd;

  ev.events = events;
  ev.data.ptr = eh;

  ret = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &ev);
  if (ret < 0) {
    free(eh);
    perror("epoll_ctl");
  }

  if (oeh)
    *oeh = eh;

  return 0;
}

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
      ret = eh->rh(eh);

      // fixme: remote disconnect

      if (ret < 0)
        herror("eh->rh");
    }
    if (evi->events & EPOLLOUT) {
      ret = eh->wh(eh);
      if (ret < 0)
        herror("eh->rh");
    }
  }

  return 0;
}
