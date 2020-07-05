#include <math.h>
#include "list.h"
#include "multithreading.h"

/**
 * factorize_even_numbers - helper function to divide a number by 2
 * @list: linked list to store each prime factor
 * @n: number which needs to be factorized
 * Return: linked list where every nodes contains one prime factor
 */
void factorize_even_numbers(list_t *list, unsigned long *n)
{
	unsigned long *m;

	while (*n % 2 == 0)
	{
		*n /= 2;
		m = malloc(sizeof(*m));
		if (!m)
		{
			list_destroy(list, free);
			return;
		}
		*m = 2;
		list_add(list, (void *)m);
	}
}

/**
 * factorize - helper function to find prime factors of a number
 * Implementing Newton's method to find the square root of a number
 * @list: linked list to store each prime factor
 * @n: number which needs to be factorized
 * Return: linked list where every nodes contains one prime factor of a number
*/
list_t *factorize(list_t *list, unsigned long n)
{
	unsigned long i;
	unsigned long *m;
	double x;

	factorize_even_numbers(list, &n);
	x = n;
	while (x * x > n)
		x = (unsigned long)((x + n / x) / 2);
	for (i = 3; i <= (unsigned long)x;)
	{
		if (n % i == 0)
		{
			n /= i;
			m = malloc(sizeof(*m));
			if (!m)
			{
				list_destroy(list, free);
				return (NULL);
			}
			*m = i;
			list_add(list, (void *)m);
		}
		else
			i += 2;
	}
	if (n > 1)
	{
		m = malloc(sizeof(*m));
		if (!m)
		{
			list_destroy(list, free);
			return (NULL);
		}
		*m = n;
		list_add(list, (void *)m);
	}
	return (list);
}

/**
 * prime_factors - function to calculate prime factors of a number
 * @s: unsigned long given as a string
 * Return: linked list where every nodes contains one prime factor of a number
*/
list_t *prime_factors(char const *s)
{
	list_t *list;
	unsigned long n;
	char *non_digit_ptr = NULL;

	if (!s)
		return (NULL);
	n = strtoul(s, &non_digit_ptr, 10);
	if (errno == EINVAL || errno == ERANGE || !non_digit_ptr)
		return (NULL);
	list = calloc(sizeof(*list), 1);
	if (!list)
		return (NULL);
	if (!factorize(list, n))
	{
		list_destroy(list, free);
		return (NULL);
	}
	return (list);
}
