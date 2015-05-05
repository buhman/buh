#pragma once

#include "event.h"

int
buh_accept(int efd, int afd,
           event_handler **eh);

int
buh_bind_inet(int efd, unsigned short port,
              event_handler **eh);

int
buh_bind_unix(int efd, const char *path,
              event_handler **eh);

int
buh_connect_unix(int efd, const char *path,
                 event_handler **eh);

int
buh_connect_inet(int efd, const char *node, const char *service,
                 event_handler **eh);
