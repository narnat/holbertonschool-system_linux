#include "signals.h"

/**
 * signal_handler - Action to take in case signal
 * Return: 0 on success, or -1 on error
*/
void signal_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
}

/**
 * handle_signal - handles SIGINT signal
 * Return: 0 on success, or -1 on error
*/
int handle_signal(void)
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return (-1);
	return (0);
}
