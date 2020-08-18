#ifndef __REST_API_H_
#define __REST_API_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BACKLOG 10

int init_inet_socket(void);
int print_raw_req(char *header, int sck);

#endif  /* __REST_API_H_ */
