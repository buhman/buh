#include <string.h>

#include <sys/epoll.h>

#include <buh/buh.h>
#include <buh/net/event.h>
#include <buh/net/queue.h>

int
buh_net_queue_send(event_handler *eh, void *ptr, size_t size)
{
  int ret;
  buf_t *buf;

  buf = vec_buf_push(&eh->out.queue);
  buf->ptr = malloc(size);
  buf->size = size;
  memcpy(buf->ptr, ptr, buf->size);

  if (eh->ev.events & EPOLLOUT)
    return 0;

  eh->ev.events |= EPOLLOUT;
  ret = epoll_ctl(eh->efd, EPOLL_CTL_MOD, eh->sfd, &eh->ev);
  if (ret < 0)
    herror("epoll_ctl");

  return 0;
}
