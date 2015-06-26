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
      fprintf(stderr, "%d disconnect\n", eh->sfd);
      if (eh->in.close)
        eh->in.close(eh);
      buh_event_close(eh);
      return 0;
    }

    fprintf(stderr, "%d recv: %zd\n", eh->sfd, len);
    ret = eh->in.recv(eh, buf, len);
    if (ret < 0) /* FIXME */
      return -1;
  }

  return 0;
}

int
buh_net_send(event_handler *eh)
{
  buf_t *buf;
  size_t index;
  ssize_t ret;

  vec_foreach(index, buf, eh->out.queue) {

    ret = send(eh->sfd, buf->ptr, buf->size, 0);
    fprintf(stderr, "%d send: %zd\n", eh->sfd, buf->size);
    if (ret < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        vec_buf_slide(&eh->out.queue, index);
        return 0;
      }
      herror("send");
    }

    free(buf->ptr);
  }

  eh->out.queue.count = 0;

  if (!(eh->ev.events & EPOLLOUT))
    return 0;

  eh->ev.events &= ~EPOLLOUT;
  ret = epoll_ctl(eh->efd, EPOLL_CTL_MOD, eh->sfd, &eh->ev);
  if (ret < 0)
    herror("epoll_ctl");

  return 0;
}
