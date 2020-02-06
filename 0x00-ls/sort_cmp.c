#include "ls.h"

/**
 * sort_size - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_size(container_t *f1, container_t *f2)
{
	if (f1->sb.st_size == f2->sb.st_size)
		return (sort_regular(f1, f2));
	return (f1->sb.st_size >= f2->sb.st_size ? 1 : 0);
}

/**
 * sort_size_rev - sort files by size in reverse
 * @f1: argument struct
 * @f2: Array
 * Return: 0 if f2 > f1, 1 if otherwise
 */
int sort_size_rev(container_t *f1, container_t *f2)
{
	if (f1->sb.st_size == f2->sb.st_size)
		return (sort_reverse(f1, f2));
	return (f1->sb.st_size >= f2->sb.st_size ? 0 : 1);
}


/**
 * sort_time - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_time(container_t *f1, container_t *f2)
{
	if (f1->sb.st_mtim.tv_sec == f2->sb.st_mtim.tv_sec)
	{
		if (f1->sb.st_mtim.tv_nsec == f2->sb.st_mtim.tv_nsec)
		{
			return (sort_regular(f1, f2));
		}
		return (f1->sb.st_mtim.tv_nsec >= f2->sb.st_mtim.tv_nsec ? 1 : 0);
	}
	return (f1->sb.st_mtim.tv_sec >= f2->sb.st_mtim.tv_sec ? 1 : 0);
}

/**
 * sort_time_rev - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_time_rev(container_t *f1, container_t *f2)
{
	if (f1->sb.st_mtim.tv_sec == f2->sb.st_mtim.tv_sec)
	{
		if (f1->sb.st_mtim.tv_nsec == f2->sb.st_mtim.tv_nsec)
		{
			return (sort_reverse(f1, f2));
		}
		return (f1->sb.st_mtim.tv_nsec >= f2->sb.st_mtim.tv_nsec ? 0 : 1);
	}
	return (f1->sb.st_mtim.tv_sec >= f2->sb.st_mtim.tv_sec ? 0 : 1);
}

/**
 * _strcmp - compares two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: returns the differences of ASCII characters
 */

int _strcmp(char *s1, char *s2)
{
	char *tmp;

	tmp = _strrchr(s1, '/');
	if (tmp)
		s1 = tmp, s1++;

	tmp = _strrchr(s2, '/');

	if (tmp)
		s2 = tmp, s2++;

	while (s1 && *s1 == '.')
		s1++;

	while (s2 && *s2 == '.')
		s2++;

	return (_strcmp2(s1, s2));
}
