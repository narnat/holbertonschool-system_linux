#include "strace.h"
#include "syscalls.h"
#include <unistd.h>

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
 * print_register - print registers (params of syscalls)
 * @u_in: registers struct
 * @idx: parameter index
 * @reg: register
 * @str: delimeter
 * @child_pid: PID of child process
 */
void print_register(struct user_regs_struct u_in, int idx, ulong reg,
		    char *str, pid_t child_pid)
{
	char *s = NULL;

	if (syscalls_64_g[u_in.orig_rax].params[idx] != (type_t)-1 &&
	    syscalls_64_g[u_in.orig_rax].params[idx] != VOID)
	{
		if (syscalls_64_g[u_in.orig_rax].params[idx] == VARARGS)
			fprintf(stdout, "%s...", str);
		else if (reg == 0 &&
			 syscalls_64_g[u_in.orig_rax].params[idx] >= PTR_PRM_STRT &&
			 syscalls_64_g[u_in.orig_rax].params[idx] <= PTR_PRM_END)
			fprintf(stdout, "%sNULL", str);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == CHAR_P)
		{
			s = read_string(child_pid, reg);
			fprintf(stdout, "%s\"%s\"", str, s);
			free(s);
		}
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == INT)
			fprintf(stdout, "%s%d", str, (int)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == LONG)
			fprintf(stdout, "%s%ld", str, (long)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == SIZE_T)
			fprintf(stdout, "%s%lu", str, (ulong)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == SSIZE_T)
			fprintf(stdout, "%s%ld", str, (long)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == U64)
			fprintf(stdout, "%s%lu", str, (ulong)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == UINT32_T)
			fprintf(stdout, "%s%lu", str, (ulong)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == UNSIGNED_INT)
			fprintf(stdout, "%s%u", str, (uint)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == UNSIGNED_LONG)
			fprintf(stdout, "%s%lu", str, (ulong)reg);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == PID_T)
			fprintf(stdout, "%s%d", str, (int)reg);
		else
			fprintf(stdout, "%s%#lx", str, reg);
	}
}

void print_access_flgs(ulong rsi)
{
	int count = 0;

	if (rsi & F_OK)
	{
		printf(", F_OK");
		return;
	}
	if (rsi & R_OK)
		count += printf(", R_OK");
	if (rsi & W_OK)
		count += printf("%sW_OK", count > 0 ? "|" : ", ");
	if (rsi & X_OK)
		count += printf("%sX_OK", count > 0 ? "|" : ", ");
}

void print_open_flgs(ulong rsi)
{

}

/**
 * print_args - print arguments to syscalls
 * @u_in: registers struct
 * @child_pid: PID of child process
 */
void print_args(struct user_regs_struct u_in, pid_t child_pid)
{
	print_register(u_in, 0, u_in.rdi, "", child_pid);
	if (syscalls_64_g[u_in.orig_rax].nr == 21)
		print_access_flgs(u_in.rsi);
	else if (syscalls_64_g[u_in.orig_rax].nr == 2)
		print_open_flgs(u_in.rsi);
	else
		print_register(u_in, 1, u_in.rsi, ", ", child_pid);
	print_register(u_in, 2, u_in.rdx, ", ", child_pid);
	print_register(u_in, 3, u_in.r10, ", ", child_pid);
	print_register(u_in, 4, u_in.r8, ", ", child_pid);
	print_register(u_in, 5, u_in.r9, ", ", child_pid);
}

/**
 * tracer - tracer process
 * @child: subprocess PID
 * @argc: arg count
 * @argv: argument vector
 * @envp: environmental variables
 * Return: EXIT_SUCCESS on success
 */
int tracer(pid_t child, int argc, char *argv[], char *envp[])
{
	int status, i;
	long retval;
	struct user_regs_struct u_in;

	setbuf(stdout, NULL);
	waitpid(child, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

	if (wait_syscall(child) != 0)
		return (0);
	ptrace(PTRACE_GETREGS, child, 0, &u_in);
	if (wait_syscall(child) != 0)
		return (0);
	retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
	fprintf(stdout, "execve(\"%s\", [", argv[0]);
	for (i = 0; i < argc; ++i)
		printf("%s\"%s\"", i == 0 ? "" : ", ", argv[i]);
	for (i = 0; envp[i]; ++i)
		;
	printf("], [/* %d vars */]) = %#lx\n", i, retval);
	while (1)
	{
		if (wait_syscall(child) != 0)
			break;
		ptrace(PTRACE_GETREGS, child, 0, &u_in);
		fprintf(stdout, "%s(", syscalls_64_g[u_in.orig_rax].name);
		print_args(u_in, child);
		if (wait_syscall(child) != 0)
			break;
		retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
		print_retval(retval, u_in);
	}
	fprintf(stdout, ") = ?\n");
	return (EXIT_SUCCESS);
}

/**
 * main - Entry point
 * @argc: Args count
 * @argv: Args vector
 * @envp: Environmental variables
 * Return: EXIT_FAILURE if failed, EXIT_SUCCESS if successful
 */
int main(int argc, char *argv[], char *envp[])
{
	pid_t child;

	if (argc < 2)
	{
		fprintf(stderr, "%s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	child = fork();
	++argv;
	--argc;
	if (child < 0)
	{
		return (EXIT_FAILURE);
	}
	if (child == 0)
	{
		ptrace(PTRACE_TRACEME);
		kill(getpid(), SIGSTOP);
		return (execve(argv[0], argv, envp));
	}
	else
	{
		return (tracer(child, argc, argv, envp));
	}
}
