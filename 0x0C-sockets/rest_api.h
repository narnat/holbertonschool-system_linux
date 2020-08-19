#ifndef __REST_API_H_
#define __REST_API_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "data_structures.h"

#define PORT 8080
#define BACKLOG 10
#define FIELD_SIZE 256
#define JSON_SIZE 256

int init_inet_socket(void);
int print_raw_req(char *header, int sck);
void send_err(int sck, int err_code);
int get_queries(int sck, char *body, char **title, char **desc);
void serialize(char *buf, size_t size, todo_list *node);
void send_created(int sck, queue_t *queue);


#endif  /* __REST_API_H_ */
