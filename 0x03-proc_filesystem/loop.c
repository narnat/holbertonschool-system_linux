#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	char *str;
	char *word = "Holberton";
	size_t i = 0;
	pid_t pid = getpid();

	str = strdup(word);

	for (;; ++i)
	{
		printf("[%lu] [pid: %d] %s [%p]\n", i, pid, str, &str);
		sleep(1);
	}
	return 0;
}
