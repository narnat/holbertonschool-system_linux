#include "_getline.h"

/**
 * free_descriptor - frees a descriptor_t list
 * @head: pointer to list to be freed
 * Return: void
 */
void free_descriptor(descriptor_t *head)
{
	descriptor_t *current;

	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}
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
	size_t i, res_size = 0, old_size = 0;
	char *res = NULL;
	ssize_t read_val;

	for (i = desc->pos; i < READ_SIZE; i++)
	{
		if (desc->buf[i] == '\n')
		{
			old_size = res_size;
			res_size += sizeof(char) * (i - desc->pos + 1);
			res = _realloc(res, old_size, res_size);
			memcpy(res, desc->buf + desc->pos, i - desc->pos);
			res[i - desc->pos] = '\0';
			desc->pos = ++i;
			return (res);
		}

		if (i == READ_SIZE - 1)
		{
			old_size = res_size;
			res_size += READ_SIZE;
			res = _realloc(res, old_size, res_size);
			memcpy(res, desc->buf + desc->pos, i - desc->pos);
			memset(desc->buf, 0, READ_SIZE);
			read_val = read(desc->fd, desc->buf, READ_SIZE);
			if (!read_val)
			{
				free(res);
				return (NULL);
			}
			desc->pos = i = 0;
		}
	}
	free(res);
	return (NULL);
}

/**
 * _getline - getline function
 * @fd: file descriptor
 * Return: A line read from @fd
 */
char *_getline(const int fd)
{
	static descriptor_t list = {0, {0}, 0, NULL};
	descriptor_t *cur = NULL;

	if (fd == -1)
	{
		free_descriptor(list.next);
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
