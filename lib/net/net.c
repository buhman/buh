#include "config.h"

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>

#include "buh/buh.h"
#include "buh/net/net.h"
#include "buh/net/event.h"

/**
 * @ingroup net
 * @{
 */

/**
 * Accept all pending connections, creating a new event handler for
 * each connection
 * @param efd epoll file descriptor
 * @param afd accept/listen file descriptor
 * @param eh return event handler
 * @returns nonzero on failure
 */
int
buh_accept(int efd, int afd,
           event_handler **eh)
{
  int sfd, ret;

  while (true) {
    sfd = accept4(afd, NULL, NULL, SOCK_NONBLOCK);
    if (sfd < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      herror("accept4");
    }

    ret = buh_event_add(efd, sfd, EPOLLIN, eh);
    if (ret < 0)
      herror("buh_event_add");
  }

  return 0;
}

/* bind */

static int
lbind(int efd, int sfd, struct sockaddr *sa, socklen_t sa_len,
      event_handler **eh)
{
  int ret;

  ret = bind(sfd, sa, sa_len);
  if (ret < 0)
    herror("bind");

  ret = listen(sfd, 1024);
  if (ret < 0)
    herror("listen");

  ret = buh_event_add(efd, sfd, EPOLLIN, eh);
  if (ret < 0)
    herror("buh_event_add");

  return 0;
}

/**
 * Bind a new listening inet socket
 * @param efd epoll file descriptor
 * @param port inet port number to listen on
 * @param eh return event handler
 * @returns nonzero on failure
 */
int
buh_bind_inet(int efd, unsigned short port,
              event_handler **eh)
{
  int sfd, ret = 1;
  struct sockaddr_in6 sa = {
    .sin6_family = AF_INET6,
    .sin6_port = htons(port),
    .sin6_addr = IN6ADDR_ANY_INIT
  };

  sfd = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (sfd < 0)
    herror("socket");

  ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(ret));
  if (ret < 0)
    herror("SO_REUSEADDR");

  ret = lbind(efd, sfd, (struct sockaddr*)&sa, sizeof(sa), eh);
  if (ret < 0)
    herror("lbind");

  return sfd;
}

/**
 * Bind a new listening unix socket
 * @param efd epoll file descriptor
 * @param path socket path
 * @param eh return event handler
 * @returns nonzero on failure
 */
int
buh_bind_unix(int efd, const char *path,
              event_handler **eh)
{
  int ret, sfd;
  struct sockaddr_un sa = {
    .sun_family = AF_UNIX,
  };

  sfd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_NONBLOCK, 0);
  if (sfd < 0)
    herror("socket");

  strncpy(sa.sun_path, path, sizeof(sa.sun_path) - 1);

  ret = lbind(efd, sfd, (struct sockaddr*)&sa, sizeof(sa), eh);
  if (ret < 0)
    herror("lbind");

  return sfd;
}

/* connect */

/**
 * Connect new socket to existing unix socket
 * @param efd epoll file descriptor
 * @param path socket path
 * @param eh return event handler
 * @returns nonzero on failure
 */
int
buh_connect_unix(int efd, const char *path,
                 event_handler **eh)
{
  int ret, sfd;
  struct sockaddr_un sa = {
    .sun_family = AF_UNIX,
  };

  sfd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_NONBLOCK, 0);
  if (sfd < 0)
    herror("socket");

  strncpy(sa.sun_path, path, sizeof(sa.sun_path) - 1);

  ret = connect(sfd, (struct sockaddr*)&sa, sizeof(struct sockaddr_un));
  if (ret < 0)
    herror("connect");

  ret = buh_event_add(efd, sfd, EPOLLIN, eh);
  if (ret < 0)
    herror("buh_event_add");

  return sfd;
}

/**
 * Connect new socket to existing inet socket
 * @param efd epoll file descriptor
 * @param node internet host
 * @param service internet service
 * @param eh return event handler
 * @returns nonzero on failure
 */
int
buh_connect_inet(int efd, const char *node, const char *service,
                 event_handler **eh)
{
  int ret, sfd = 0;
  struct addrinfo *res, *resi, hint = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
  };

  ret = getaddrinfo(node, service, &hint, &res);
  if (ret < 0)
    herror("getaddrinfo");

  for (resi = res; resi != NULL; resi = resi->ai_next) {

    sfd = socket(resi->ai_family, resi->ai_socktype | SOCK_NONBLOCK, resi->ai_protocol);
    if (sfd < 0) {
      perror("socket");
      continue;
    }

    ret = connect(sfd, resi->ai_addr, resi->ai_addrlen);
    if (ret >= 0 || errno == EINPROGRESS)
      break;

    perror("connect");
    close(sfd);
  }

  freeaddrinfo(res);

  ret = buh_event_add(efd, sfd, EPOLLIN, eh);
  if (ret < 0)
    herror("buh_event_add");

  return sfd;
}
