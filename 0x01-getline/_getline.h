#ifndef GETLINE_H
#define GETLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define READ_SIZE 256

/**
 * struct descriptor_s - singly linked list  of file descriptors
 * @fd: file descriptor
 * @buf: character buffer
 * @pos: current position of the buffer
 * @flush: flush the buffer
 * @next: points to the next node
 *
 * Description: singly linked list node structure,
 * which holds a file descirptor
 */
typedef struct descriptor_s
{
	int fd;
	char buf[READ_SIZE];
	size_t pos;
	int flush;
	struct descriptor_s *next;
} descriptor_t;

char *_getline(const int fd);

#endif /* GETLINE_H */
