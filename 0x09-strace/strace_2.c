#include "strace.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/reg.h>

int sub_process(int argc, char *argv[])
{
	(void) argc;
	ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
	return execve(argv[0], argv, NULL);
}

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

int tracer(pid_t child)
{
	int status;
	long syscall, retval;

	waitpid(child, &status, 0);

	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		if (wait_syscall(child) != 0)
			break;
		syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX);
		fprintf(stderr, "%s = ", syscalls_64_g[syscall].name);
		if (wait_syscall(child) != 0) break;
		retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
		fprintf(stderr, "%#lx\n", retval);
	}
	fprintf(stderr, "?\n");
	return (EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	pid_t child;

	if (argc < 2)
	{
		fprintf(stderr, "%s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	child = fork();

	if (child == 0)
	{
		return (sub_process(argc - 1, argv + 1));
	}
	else
	{
		return (tracer(child));
	}
}
