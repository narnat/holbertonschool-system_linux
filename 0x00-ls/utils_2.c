#include "ls.h"

/**
 * swap - Swaps two numbers
 * @f1: First number
 * @f2: Second number
 * Return: Nothing
 */
void swap(container_t *f1, container_t *f2)
{
	container_t temp;

	temp = *f1;
	*f1 = *f2;
	*f2 = temp;
}

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

/**
 * _strrchr - locates last occurence of the character
 * @s: string
 * @c: character
 *
 * Return: returns a pointer
 */

char *_strrchr(char *s, char c)
{
	char *last = NULL;

	while (*s)
	{
		if (*s == c)
			last = s;
		s++;
	}
	return (last);
}

/**
 * _tolower - converts uppercase to lowercase
 * @c: takes character in ASCII
 *
 * Return: lowercase
 */

char _tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}
