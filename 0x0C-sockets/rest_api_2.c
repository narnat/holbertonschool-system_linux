#include "rest_api.h"

/**
 * send_created - send 201 code with JSON body
 * @sck: socket fd
 * @queue: queue of todo tasks
 */
void send_created(int sck, queue_t *queue)
{
	char buf[JSON_SIZE];

	serialize(buf, JSON_SIZE, queue->last);
	dprintf(sck,
			"HTTP/1.1 201 Created\r\n"
			"Content-Length: %lu\r\nContent-Type: application/json\r\n\r\n%s",
			strlen(buf), buf);
}
