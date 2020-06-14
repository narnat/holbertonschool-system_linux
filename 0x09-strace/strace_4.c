#include "strace.h"
#include "syscalls.h"

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
			fprintf(stderr, "%s...", str);
		else if (syscalls_64_g[u_in.orig_rax].params[idx] == CHAR_P)
		{
			s = read_string(child_pid, reg);
			fprintf(stderr, "%s\"%s\"", str, s);
			free(s);
		}
			else
				fprintf(stderr, "%s%#lx", str, reg);
	}
}

/**
 * print_args - print arguments to syscalls
 * @u_in: registers struct
 * @child_pid: PID of child process
 */
void print_args(struct user_regs_struct u_in, pid_t child_pid)
{
	fprintf(stderr, "(");
	print_register(u_in, 0, u_in.rdi, "", child_pid);
	print_register(u_in, 1, u_in.rsi, ", ", child_pid);
	print_register(u_in, 2, u_in.rdx, ", ", child_pid);
	print_register(u_in, 3, u_in.r10, ", ", child_pid);
	print_register(u_in, 4, u_in.r8, ", ", child_pid);
	print_register(u_in, 5, u_in.r9, ", ", child_pid);
	fprintf(stderr, ")");
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
		printf("\"%s\"", argv[i]);
	for (i = 0; envp[i]; ++i)
		;
	printf("], /* %d vars*/) = %#lx\n", i, retval);

	while (1)
	{
		if (wait_syscall(child) != 0)
			break;
		ptrace(PTRACE_GETREGS, child, 0, &u_in);
		fprintf(stderr, "%s", syscalls_64_g[u_in.orig_rax].name);
		print_args(u_in, child);
		if (wait_syscall(child) != 0)
			break;
		retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
		fprintf(stderr, " = %#lx\n", retval);
	}
	fprintf(stderr, " = ?\n");
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
