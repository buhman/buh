#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> /* FIXME */

#include <sys/socket.h>
#include <sys/epoll.h>

#include <buh/buh.h>
#include <buh/vector.h>
#include <buh/net/event.h>
#include <buh/net/handler.h>

int
buh_net_recv(event_handler *eh)
{
  char buf[4096];
  ssize_t len;
  int ret;

  while (true) {
    len = recv(eh->sfd, buf, sizeof buf, 0);
    if (len < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      herror("recv");
    } else if (len == 0) {
      printf("disconnect\n");
      buh_event_close(eh);
      return 0;
    }

    fprintf(stderr, "recv: %lld\n", len);
    ret = eh->in.recv(eh, buf, len);
    if (ret < 0) /* FIXME */
      return -1;
  }

  return 0;
}

int
buh_net_send(event_handler *eh)
{
  vector_item_t *item;
  size_t index;
  ssize_t ret;

  for (index = 0; index < eh->out.queue.size; ++index) {

    item = eh->out.queue.items + index;
    ret = send(eh->sfd, item->buf, item->size, 0);
    fprintf(stderr, "send: %lld\n", item->size);
    if (ret < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        vec_slide(&eh->out.queue, index);
        return 0;
      }
      herror("send");
    }

    free(item->buf);
  }

  eh->out.queue.index = 0;

  if (!(eh->ev.events & EPOLLOUT))
    return 0;

  eh->ev.events &= ~EPOLLOUT;
  ret = epoll_ctl(eh->efd, EPOLL_CTL_MOD, eh->sfd, &eh->ev);
  if (ret < 0)
    herror("epoll_ctl");

  return 0;
}
