#include "ls.h"

/**
 * print_link_src - prints source of the symlink
 * @args: arguments
 * @file: file struct
 * Return: Void
 */
void print_link_src(args_t *args, container_t *file)
{
	char buf[BUFSIZ] = {0};

	if (args->opt.show_info && (file->sb.st_mode & S_IFMT) == S_IFLNK)
	{
		readlink(file->name, buf, BUFSIZ);
		printf(" -> %s", buf);
	}
}


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
	char *name = NULL;

	sort(args, files, size);
	for (files_i = 0; files_i < size; files_i++)
	{
		if (!args->strip_path && ((files[files_i].sb.st_mode & S_IFMT) == S_IFDIR))
			continue;
		if (args->strip_path)
			name = _strrchr(files[files_i].name, '/');
		flag = 1;
		print_info(args, &(files[files_i]), width);
		printf("%s", name ? ++name : files[files_i].name);
		print_link_src(args, &files[files_i]);
		if (files_i != size - 1 || args->opt.put_newline)
			printf("%s", separator);
	}
	if (flag && !args->opt.put_newline)
		printf("\n");
	args->strip_path = 1;
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
		read_files(args, node_i, 1, idx);
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
	if (((idx != 0) || (idx == 0 && (args->n_files - args->n_dirs) > 0))
		&& errno != EACCES)
	{
		if (errno == EACCES)
			errno = 0;
		printf("\n");
	}
	else if ((idx != 0 || args->opt.is_recursing)
			 && args->strip_path && args->opt.show_nested)
		printf("\n");
	if (args->err > 0 || args->n_files > 1 || (args->opt.show_nested))
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
	width[1] = max(width[1], getpwuid(sb.st_uid)
				   ? _strlen(getpwuid(sb.st_uid)->pw_name) : number_width(sb.st_uid));
	width[2] = max(width[2], getgrgid(sb.st_gid)
				   ? _strlen(getgrgid(sb.st_gid)->gr_name) : number_width(sb.st_gid));
	width[3] = max(width[3], number_width(sb.st_size));
}
