#include "multithreading.h"

pthread_mutex_t lock;

/**
 * mutex_init - this function called before main function to
 * initialize lock variable (Constructor)
*/
void mutex_init(void)
{
	if (pthread_mutex_init(&lock, NULL))
	{
		printf("Mutex init has failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * tprintf - prepends caller's Thread ID and calls vprintf function
 * Thread-safe version (mutex lock)
 * @format: formatting string
 * Return: number of printed characters
*/
int tprintf(char const *format, ...)
{
	va_list args;
	int count = 0;

	va_start(args, format);
	if (pthread_mutex_lock(&lock))
	{
		printf("Mutex lock has failed\n");
		exit(EXIT_FAILURE);
	}
	count += printf("[%lu] ", pthread_self());
	count += vprintf(format, args);
	if (pthread_mutex_unlock(&lock))
	{
		printf("Mutex unlock has failed\n");
		exit(EXIT_FAILURE);
	};
	va_end(args);
	return (count);
}
