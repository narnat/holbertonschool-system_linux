#include "ls.h"

/**
 * print_files - prints files in sorted order and frees
 * @args: arguments
 * @files: Array of files
 * @size: size of files
 * @width: width array
 * Return: Void
 */
void print_files(args_t *args, container_t *files, int size, size_t *width)
{
	int files_i, flag = 0;
	char *separator = args->opt.put_newline ? "\n" : "\t";

	for (files_i = 0; files_i < size; files_i++)
	{
		flag = 1;
		print_info(args, &(files[files_i]), width);
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
	if (((idx != 0) || (idx == 0 && args->n_files > 0)) && errno != EACCES)
	{
		if (errno == EACCES)
			errno = 0;
		printf("\n");
	}
	if (args->err > 0 || args->n_files + args->n_dirs > 1)
	{
		printf("%s:\n", name);
	}
}

/**
 * get_info_width - get the max width of links, user, group and size
 * @width: arr of size 4, each meaning max width for link, user, group, sizeof
 * @sb: struct stat file info
*/
void get_info_width(size_t *width, stat_t sb)
{
	width[0] = max(width[0], number_width(sb.st_nlink));
	width[1] = max(width[1], _strlen(getpwuid(sb.st_uid) ?
									 getpwuid(sb.st_uid)->pw_name : ""));
	width[2] = max(width[2], _strlen(getgrgid(sb.st_gid) ?
									 getgrgid(sb.st_gid)->gr_name : ""));
	width[3] = max(width[3], number_width(sb.st_size));
}
