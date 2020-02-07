#include "ls.h"

/**
 * _strcmp_case_sensitive - compares two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: returns the differences of ASCII characters
 */

int _strcmp_case_sensitive(char *s1, char *s2)
{
	int diff, i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	diff = s1[i] - s2[i];
	return (diff);
}

/**
 * _strncmp - Compares two strings at most n bytes
 * @s1: First string being evaluated
 * @s2: Second string being evaluated
 * @n: Amount of bytes to be evaluated
 *
 * Return: Difference between string characters being evaluated
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;

	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] == s2[i])
			++i;
		else
			return (s1[i] - s2[i]);
	}
	return (0);
}
