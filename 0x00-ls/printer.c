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
	int files_i;
	char *separator = args->opt.put_newline ? "\n" : "\t";

	for (files_i = 0; files_i < size; files_i++)
	{
		/* print_info(args, &(files[files_i])); */
		printf("%s%s", files[files_i].name, separator);
	}
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

	node_i = dirs->first;

	while (node_i)
	{
		read_files(args, node_i, 100);
		node_i = node_i->next;
	}

	free_queue(&dirs);
}
