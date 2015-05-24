#include <string.h>

#include <sys/epoll.h>

#include <buh/buh.h>
#include <buh/net/event.h>
#include <buh/net/queue.h>

int
buh_net_queue_send(event_handler *eh, void *buf, size_t size)
{
  int ret;
  char *ptr;

  ptr = malloc(size);
  memcpy(ptr, buf, size);
  vec_push(&eh->out.queue, ptr, size);

  if (eh->ev.events & EPOLLOUT)
    return 0;

  eh->ev.events |= EPOLLOUT;
  ret = epoll_ctl(eh->efd, EPOLL_CTL_MOD, eh->sfd, &eh->ev);
  if (ret < 0)
    herror("epoll_ctl");

  return 0;
}
