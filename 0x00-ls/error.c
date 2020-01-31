#include "ls.h"

/**
 * free_arr - frees array of files
 * @files: - files array
 * @size: size of the array
 * Return: Void
 */
void free_arr(container_t *files, int size)
{
	int files_i;

	for (files_i = 0; files_i < size; files_i++)
	{
		free(files[files_i].name);
	}
	free(files);
}

/**
 * error - custom error function
 * @args: - arguments
 * Return: Void
 */
void error(args_t *args)
{
	char buf[BUFSIZ];
	char *name = args->argv[0];

	fprintf(stderr, "%s: ", name);
	if (args->err == OPT_ERR)
	{
		sprintf(buf, "invalid option -- '%c'\nTry", args->argv[args->inv][1]);
		fprintf(stderr, "%s 'ls --help' for more information.\n", buf);
		exit(LS_SERIOUS);
	}
	if (args->err == FILE_ERR)
	{
		sprintf(buf, "cannot access %s", args->argv[args->inv]);
		perror(buf);
	}
	if (args->err == P_ERR)
	{
		sprintf(buf, "cannot open directory %s", args->dir_name);
		args->err = LS_SERIOUS, perror(buf);
	}
}
