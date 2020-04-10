#include "signals.h"

/**
 * signal_handler - Action to take in case signal
 * @signum: Signal number
 * Return: 0 on success, or -1 on error
 */
void signal_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}

/**
 * handle_sigaction - handles SIGINT signal
 * Return: 0 on success, or -1 on error
*/
int handle_sigaction(void)
{
	struct sigaction sig;

	sig.sa_handler = signal_handler;
	return (sigaction(SIGINT, &sig, NULL));
}
