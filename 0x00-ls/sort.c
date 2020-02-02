#include "ls.h"

/**
 * partition - Swaps two numbers
 * @arr: Array
 * @low: First number
 * @high: Second number
 * @cmp: comparator function
 * Return: Nothing
 */
int partition(container_t *arr, int low, int high,
			  int (*cmp)(container_t *f1, container_t *f2))
{
	container_t pivot = arr[high];
	int i = (low - 1), j;

	for (j = low; j <= high - 1; j++)
	{
		if (cmp(&(arr[j]), &pivot))
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
	}

	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/**
 * quickSort - Swaps two numbers
 * @arr: Array
 * @low: First number
 * @high: Second number
 * @size: Size of the @array
 * @cmp: comparator function
 * Return: Nothing
 */
void quickSort(container_t *arr, int low, int high, size_t size,
			   int (*cmp)(container_t *f1, container_t *f2))
{
	int pi;

	if (low < high)
	{
		pi = partition(arr, low, high, cmp);
		quickSort(arr, low, pi - 1, size, cmp);
		quickSort(arr, pi + 1, high, size, cmp);
	}
}

/**
 * sort - sort files arr
 * @args: argument struct
 * @arr: Array
 * @size: Size of the @array
 * Return: Nothing
 */
void sort(args_t *args, container_t *arr, size_t size)
{
	if (args->opt.sort_size && args->opt.sort_reverse)
		quickSort(arr, 0, size - 1, size,  sort_size_rev);
	else if (args->opt.sort_time && args->opt.sort_reverse)
		quickSort(arr, 0, size - 1, size, sort_time_rev);
	else if (args->opt.sort_size)
		quickSort(arr, 0, size - 1, size,  sort_size);
	else if (args->opt.sort_time)
		quickSort(arr, 0, size - 1, size, sort_time);
	else if (args->opt.sort_reverse)
		quickSort(arr, 0, size - 1, size, sort_reverse);
	else
		quickSort(arr, 0, size - 1, size, sort_regular);
}

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
