#include "strace.h"
#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
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

void print_register(struct user_regs_struct u_in, int idx, ulong reg, char *str)
{
	if (syscalls_64_g[u_in.orig_rax].params[idx] != (type_t)-1 &&
	    syscalls_64_g[u_in.orig_rax].params[idx] != VOID)
	{
		if (syscalls_64_g[u_in.orig_rax].params[idx] == VARARGS)
			fprintf(stderr, "%s...", str);
		else
			fprintf(stderr, "%s%#lx", str, reg);
	}
}

void print_args(struct user_regs_struct u_in)
{
	fprintf(stderr, "(");
	print_register(u_in, 0, u_in.rdi, "");
	print_register(u_in, 1, u_in.rsi, ", ");
	print_register(u_in, 2, u_in.rdx, ", ");
	print_register(u_in, 3, u_in.r10, ", ");
	print_register(u_in, 4, u_in.r8, ", ");
	print_register(u_in, 5, u_in.r9, ", ");
	fprintf(stderr, ")");
}

int tracer(pid_t child)
{
	int status;
	long syscall, retval;
	struct user_regs_struct u_in;

	(void) syscall;
	waitpid(child, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		if (wait_syscall(child) != 0)
			break;
		syscall = ptrace(PTRACE_GETREGS, child, 0, &u_in);
		fprintf(stderr, "%s", syscalls_64_g[u_in.orig_rax].name);
		print_args(u_in);
		if (wait_syscall(child) != 0) break;
		retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
		fprintf(stderr, " = %#lx\n", retval);
	}
	fprintf(stderr, " = ?\n");
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
