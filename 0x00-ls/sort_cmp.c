#include "ls.h"

/**
 * sort_regular - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_regular(container_t *f1, container_t *f2)
{
	return (_strcmp(f1->name, f2->name) > 0 ? 0 : 1);
}


/**
 * sort_reverse - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_reverse(container_t *f1, container_t *f2)
{
	return (_strcmp(f1->name, f2->name) > 0 ? 1 : 0);
}

/**
 * sort_size - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_size(container_t *f1, container_t *f2)
{
	return (_strlen(f1->name) > _strlen(f2->name) ? 1 : 0);
}


/**
 * sort_time - sort files arr
 * @f1: argument struct
 * @f2: Array
 * Return: 1 if f2 > f1, 0 if otherwise
 */
int sort_time(container_t *f1, container_t *f2)
{
	return (_strlen(f1->name) > _strlen(f2->name) ? 0 : 1);
}
