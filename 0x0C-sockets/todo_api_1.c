#include "rest_api.h"

/**
 * init_inet_socket - initialize socket, broadcast to all (INADDR_ANY)
 * use port PORT macro
 * Return: -1 if failed, socket descriptor
*/
int init_inet_socket(void)
{
	int sck;
	struct sockaddr_in addr;

	sck = socket(AF_INET, SOCK_STREAM, 0);
	if (sck == -1)
		return (-1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(sck, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (-1);
	listen(sck, BACKLOG);
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

/**
 * accept_connection - wait for connections from users and accept them
 * @sck: descriptor of the passive socket
 * Return: -1 if failed, 0 on success
*/
int accept_connection(int sck)
{
	int new_sck, clt_size;
	struct sockaddr_in client;
	char header[BUFSIZ] = {0};

	clt_size = sizeof(client);
	while (1)
	{
		new_sck = accept(sck, (struct sockaddr *)&client, (socklen_t *)&clt_size);
		if (new_sck < 0)
			return (-1);
		printf("Client connected: %s\n", inet_ntoa(client.sin_addr));
		if (print_raw_req(header, new_sck) == -1)
		{
			close(new_sck);
			close(sck);
			return (-1);
		}
		printf("Method: %s\n", strtok(header, " "));
		printf("Path: %s\n", strtok(NULL, " "));
		printf("Version: %s\n", strtok(NULL, " \r"));
		dprintf(new_sck, "HTTP/1.0 200 OK\r\n\r\n");
		close(new_sck);
	}
	return (0);
}

/**
 * main - socket which accepts connections
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	int sck;

	setbuf(stdout, NULL);
	sck = init_inet_socket();
	if (sck == -1)
		return (EXIT_FAILURE);
	if (accept_connection(sck) == -1)
		return (EXIT_FAILURE);
	close(sck);
	return (EXIT_SUCCESS);
}
