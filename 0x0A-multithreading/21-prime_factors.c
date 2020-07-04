#include "list.h"
#include "multithreading.h"

list_t *factorize(list_t *list, unsigned long start,
		  unsigned long end, unsigned long n)
{
	unsigned long i;
	unsigned long *m;

	for (i = start; i <= end;)
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
		{
			i += 2;
		}
	}
	return (list);
}

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
	if (!factorize(list, 2, 2, n))
	{
		list_destroy(list, free);
		return (NULL);
	}

	if (!factorize(list, 3, n, n))
	{
		list_destroy(list, free);
		return (NULL);
	}
	return (list);
}
