#include "strace.h"
#include "syscalls.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/reg.h>

/**
 * sub_process - create a subprocess
 * @argc: Args count
 * @argv: Args vector
 * Return: -1 if failed
*/
int sub_process(int argc, char *argv[])
{
	(void) argc;
	ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
	return (execve(argv[0], argv, NULL));
}

/**
 * wait_syscall - Waits for syscall invocation in subprocess
 * @child: subprocess PID
 * Return: 0 if syscall is called, 0 if subprocess is terminated
*/
int wait_syscall(pid_t child)
{
	int status;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		waitpid(child, &status, 0);
		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return (0);
		if (WIFEXITED(status))
			break;
	}
	return (1);
}

/**
 * tracer - tracer process
 * @child: subprocess PID
 * Return: EXIT_SUCCESS on success
 */
int tracer(pid_t child)
{
	int status;
	long syscall;

	waitpid(child, &status, 0);

	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		if (wait_syscall(child) != 0)
			break;
		syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX);
		fprintf(stderr, "%s\n", syscalls_64_g[syscall].name);
		if (wait_syscall(child) != 0)
			break;
	}
	return (EXIT_SUCCESS);
}

/**
 * main - Entry point
 * @argc: Args count
 * @argv: Args vector
 * Return: EXIT_FAILURE if failed, EXIT_SUCCESS if successful
 */
int main(int argc, char *argv[])
{
	pid_t child;

	if (argc < 2)
	{
		fprintf(stderr, "%s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (access(argv[1], R_OK) == -1)
	{
		fprintf(stderr,
			"strace: Can't stat '%s': No such file or directory\n",
			argv[1]);
		return (EXIT_FAILURE);
	}
	child = fork();

	if (child < 0)
	{
		return (EXIT_FAILURE);
	}
	else if (child == 0)
	{
		return (sub_process(argc - 1, argv + 1));
	}
	else
	{
		return (tracer(child));
	}
}
