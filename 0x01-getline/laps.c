#include <limits.h>
#include "laps.h"
#include <string.h>

/**
 * _realloc - implementation
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
 * max - returns max from array
 * @arr: integer array
 * @size: size of @arr
 * Return: max element from @arr
*/
int max(int *arr, size_t size)
{
	size_t i;
	int max = INT_MIN;

	for (i = 0; i < size; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}

	return (max);
}

/**
 * race_state - Tells the state of the race
 * @id: array of cars with unique IDs
 * @size: sizeof @id
 */
void race_state(int *id, size_t size)
{
	static int *st;
	static size_t st_size;
	size_t old_size;
	size_t i, m;

	if (!size)
	{
		free(st);
		return;
	}
	old_size = st_size;
	m = max(id, size);
	st_size = st_size > m ? st_size : m;
	if (st == NULL)
	{
		st = malloc(st_size * sizeof(int));
		memset(st, -1, st_size * sizeof(int));
	}

	if (old_size && st_size > old_size)
	{
		st = _realloc(st, old_size * sizeof(int), st_size * sizeof(int));
		memset(st + old_size, -1, (st_size - old_size) * sizeof(int));
	}

	for (i = 0; i < size; i++)
	{
		if (st[id[i] - 1] == -1)
		{
			printf("Car %d joined the race\n", id[i]);
		}
		st[id[i] - 1]++;
	}

	printf("Race state:\n");
	for (i = 0; i < st_size; i++)
	{
		if (st[i] >= 0)
			printf("Car %lu [%d laps]\n", i + 1, st[i]);
	}
}
