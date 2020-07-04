#include "multithreading.h"
#include <stdarg.h>

/**
 * tprintf - prepends caller's Thread ID and calls vprintf function
 * @format: formatting string
 * Return: number of printed characters
*/
int tprintf(char const *format, ...)
{
	va_list args;
	int count = 0;

	va_start(args, format);
	count += printf("[%lu] ", pthread_self());
	count += vprintf(format, args);
	va_end(args);
	return (count);
}
