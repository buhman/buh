#pragma once

#include "buh/net/event.h"

int
buh_net_recv(event_handler *eh);

int
buh_net_send(event_handler *eh);
