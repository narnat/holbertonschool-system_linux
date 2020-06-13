#include "strace.h"

/**
 * read_string - Returns the string stored in the memory @addr
 * @child_pid: pid of the process
 * @addr: address to be looked in
 * Return: string stored in memory
 */
char *read_string(pid_t child_pid, unsigned long addr)
{
	char *str = malloc(4096);
	unsigned int allocated = 4096;
	unsigned long read = 0;
	unsigned long tmp;

	while (1)
	{
		if (read + sizeof(tmp) > allocated)
		{
			allocated *= 2;
			str = realloc(str, allocated);
		}
		tmp = ptrace(PTRACE_PEEKDATA, child_pid, addr + read);
		if (errno != 0)
		{
			str[read] = 0;
			break;
		}
		memcpy(str + read, &tmp, sizeof(tmp));
		if (memchr(&tmp, 0, sizeof(tmp)) != NULL)
			break;
		read += sizeof(tmp);
	}
	return (str);
}
