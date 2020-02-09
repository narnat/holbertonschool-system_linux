#include "_getline.h"

/**
 * _strchr - locates a character in a string
 * @s: string
 * @c: character
 *
 * Return: returns a pointer
 */

char *_strchr(char *s, char c)
{
	while (*s && *s != c)
	{
		s++;
	}
	if (*s == c)
		return (s);
	return (NULL);
}

/**
 * get_fd - get id of the car
 * @head: linked list of cars
 * @fd: the id to look for
 * Return: the node with given @id or NULL
 */
descriptor_t *get_fd(descriptor_t *head, int fd)
{
	while (head)
	{
		if (head->fd == fd)
			return (head);
		head = head->next;
	}

	return (NULL);
}

/**
 * _realloc - realloc() implementation
 * @ptr: pointer to the memory
 * @old_size: old size of the block of memory
 * @new_size: new size of the block of memory
 *
 * Return: void
 */

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *s, *a;
	unsigned int i, limit;

	if (old_size == new_size)
		return (ptr);
	if (ptr != NULL && new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	s = malloc(sizeof(char) * new_size);
	if (!s)
		return (NULL);
	a = ptr;
	limit = new_size > old_size ? old_size : new_size;
	for (i = 0; i < limit; i++)
	{
		s[i] = a[i];
		/* free(a + i); */
	}
	free(ptr);
	/* ptr = NULL; */
	return (s);
}

/**
 * read_descriptor - read function
 * @desc: file descriptor
 * Return: A line read from @fd
 */
char *read_descriptor(descriptor_t *desc)
{
	size_t line_size = 0, line_pos = 0, free_space, needed_space;
	char *line = NULL, *temp;
	ssize_t read_val;

	while (1)
	{
		temp = _strchr(desc->buf + desc->pos, '\n');
		if (!temp)
		{
			free_space = line_size - line_pos;
			needed_space = READ_SIZE - desc->pos; /*Max READ_SIZE*/
			if (needed_space > free_space)
			{
				line = _realloc(line, line_size, line_size + READ_SIZE + 2);
				line_size += READ_SIZE;
				memset(line + line_pos + needed_space, 0, READ_SIZE - needed_space + 2);
			}
			memcpy(line + line_pos, desc->buf + desc->pos, needed_space);
			line_pos += needed_space;
			read_val = read(desc->fd, desc->buf, READ_SIZE);
			if (read_val < 1)
			{
				if (desc->flush)
				{
					desc->flush = 0;
					return (line);
				}
				free(line);
				return (NULL);
			}
			if (read_val < READ_SIZE)
				memset(desc->buf + (read_val), 0, (READ_SIZE - read_val));
			desc->pos = 0;
		}
		else
		{
			*temp = '\0', free_space = line_size - line_pos;
			needed_space = temp - (desc->buf + desc->pos) + 1; /*Max READ_SIZE + 1*/
			if (needed_space > free_space)
			{
				line = _realloc(line, line_size, line_size + READ_SIZE);
				line_size += READ_SIZE;
				memset(line + line_pos + needed_space, 0, READ_SIZE - needed_space);
			}
			memcpy(line + line_pos, desc->buf + desc->pos, needed_space);
			desc->pos += temp - (desc->buf + desc->pos) + 1;
			return (line);
		}
	}
	return (NULL);
}

/**
 * _getline - getline function
 * @fd: file descriptor
 * Return: A line read from @fd
 */
char *_getline(const int fd)
{
	static descriptor_t list = {0, {0}, 0, 1, NULL};
	descriptor_t *cur = NULL, *tmp;

	if (fd < 0)
		return (NULL);
	if (fd == -1)
	{
		cur = list.next;
		while (cur != NULL)
		{
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
		return (NULL);
	}

	if (list.fd == 0)
	{
		list.fd = fd;
		read(fd, list.buf, READ_SIZE);
	}
	cur = get_fd(&list, fd);

	return (read_descriptor(cur));
}
