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
