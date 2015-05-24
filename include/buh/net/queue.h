#pragma once

#include <stdlib.h>

#include <buh/net/event.h>

int
buh_net_queue_send(event_handler *eh, void *buf, size_t size);
