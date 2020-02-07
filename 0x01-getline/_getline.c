#include "_getline.h"

/**
 * _getline - getline function
 * @fd: file descriptor
 * Return: A line read from @fd
 */
char *_getline(const int fd)
{
	static char *buf;
	static char *point;
	char *temp;
	static size_t size;
	ssize_t r_size = -2;
	size_t i;

	(void)r_size;
	if (!buf)
	{
		buf = malloc(READ_SIZE);
		if (!buf)
			return (NULL);
		r_size = read(fd, buf, READ_SIZE);
		size = READ_SIZE;
		point = buf;
	}
	for (i = 0; i < size; i++)
	{
		if (point[i] == '\n')
		{
			point[i] = '\0';
			temp = point;
			point = point + i + 1;
			return (temp);
		}
	}
	return (NULL);
}
