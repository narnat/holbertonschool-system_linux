#include "ls.h"

/**
 * print_files - prints files in sorted order and frees
 * @args: arguments
 * @files: Array of files
 * @size: size of files
 * Return: Void
 */
void print_files(args_t *args, container_t *files, int size)
{
	int files_i, flag = 0;
	char *separator = args->opt.put_newline ? "\n" : "\t";

	for (files_i = 0; files_i < size; files_i++)
	{
		flag = 1;
		/* print_info(args, &(files[files_i])); */
		printf("%s%s", files[files_i].name, separator);
	}
	if (flag && !args->opt.put_newline)
		printf("\n");
}


/**
 * print_dirs - prints dirs
 * @args: arguments
 * @dirs: Queue of dirs
 * Return: Void
 */
void print_dirs(args_t *args, queue_t *dirs)
{
	node_t *node_i = NULL;
	unsigned int idx = 0;

	node_i = dirs->first;

	while (node_i)
	{
		read_files(args, node_i, 100, idx);
		node_i = node_i->next;
		idx++;
	}

	free_queue(&dirs);
}

/**
 * print_dir_name - print dir name
 * @args: arguments
 * @name: name of directory to print
 * @idx: index of directory
*/
void print_dir_name(args_t *args, char *name, uint idx)
{
	if ((idx != 0) || (idx == 0 && args->n_files > 0))
	{
		printf("\n");
	}
	if (args->err > 0 || args->n_files + args->n_dirs > 1)
	{
		printf("%s:\n", name);
	}
}
