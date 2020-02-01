#include "ls.h"

/**
 * number_width - count number of digits in number
 * @n: number
 * Return: returns the number of digits in number
 */
size_t number_width(size_t n)
{
	size_t count = 0;

	while (n != 0)
	{
		count++;
		n /= 10;
	}

	return (count);
}

/**
 * max - max function
 * @n1: number 1
 * @n2: number 2
 * Return: bigger number
 */
size_t max(size_t n1, size_t n2)
{
	return (n1 >= n2 ? n1 : n2);
}
