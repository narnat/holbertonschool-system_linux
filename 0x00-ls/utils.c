#include "ls.h"

/**
 * _strdup - duplicates a string
 * @str: string
 *
 * Return: a pointer to the string
 */
char *_strdup(char *str)
{
	char *s;
	int i = 0;

	if (str == NULL)
		return (NULL);
	while (str[i])
	{
		i++;
	}
	s = malloc(sizeof(char) * i + 1);
	if (s == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i++] = '\0';
	return (s);
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
 * _strstr - locates a substring
 * @haystack: string
 * @needle: prefix
 *
 * Return: returns a pointer
 */
char *_strstr(char *haystack, char *needle)
{
	while (*haystack)
	{
		if (_strcmp(haystack, needle))
			return (haystack);
		haystack++;
	}
	return (0);
}

/**
 * _strcmp - compares two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: returns true or false
 */

int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
	}
	return (1);
}
