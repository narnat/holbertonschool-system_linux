#include "rest_api.h"
#define PORT 8080

/**
 * main - socket which accepts connections
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
	int sck, new_sck, clt_size, bytes;
	struct sockaddr_in addr, client;
	char buf[1024] = {0};

	sck = socket(AF_INET, SOCK_STREAM, 0);
	if (sck == -1)
		return (EXIT_FAILURE);
	setbuf(stdout, NULL);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(sck, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (EXIT_FAILURE);
	listen(sck, 3);
	printf("Server listening on port %d\n", PORT);
	clt_size = sizeof(client);
	while (1)
	{
		new_sck = accept(sck, (struct sockaddr *)&client, (socklen_t *)&clt_size);
		if (new_sck < 0)
			return (EXIT_FAILURE);
		printf("Client connected: %s\n", inet_ntoa(client.sin_addr));

		while ((bytes = recv(new_sck, buf, 1024, 0)) > 0)
		{
			if (bytes < 0)    /* TODO: handle multiple lines */
				return (EXIT_FAILURE);
			printf("Raw request: \"%s\"\n", buf);
			printf("Method: %s\n", strtok(buf, " "));
			printf("Path: %s\n", strtok(NULL, " "));
			printf("Version: %s\n", strtok(NULL, " \r"));
			dprintf(new_sck, "HTTP/1.0 200 OK\r\n\r\n");
			memset(buf, 0, 1024);
			close(new_sck);
		}
	}
	close(sck);
	return (0);
}
