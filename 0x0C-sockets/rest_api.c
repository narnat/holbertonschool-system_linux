#include "rest_api.h"

/**
 * init_inet_socket - initialize socket, broadcast to all (INADDR_ANY)
 * use port PORT macro
 * Return: -1 if failed, socket descriptor
*/
int init_inet_socket(void)
{
	int sck, yes = 1;
	struct sockaddr_in addr;

	sck = socket(AF_INET, SOCK_STREAM, 0);
	if (sck == -1)
		return (-1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(sck, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return (-1);
	}
	listen(sck, BACKLOG);
	if (setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("setsockopt");
		return (-1);
	}
	printf("Server listening on port %d\n", PORT);
	return (sck);
}

/**
 * print_raw_req - print raw HTTP/1.1 request from a user
 * @header: header buffer, where HTTP header info will be stored
 * @sck: socket file descriptor
 * Return: -1 if failed, 0 on success
*/
int print_raw_req(char *header, int sck)
{
	int bytes;
	char buf[BUFSIZ] = {0};

	bytes = recv(sck, header, BUFSIZ, 0);
	if (bytes == -1)
		return (-1);
	printf("Raw request: \"%s", header);
	if (bytes == BUFSIZ)
	{
		while ((bytes = recv(sck, buf, BUFSIZ, 0)) > 0)
		{
			printf("%s", buf);
			memset(buf, 0, BUFSIZ);
		}
		if (bytes == -1)
			return (-1);
	}
	printf("\"\n");
	return (0);
}
