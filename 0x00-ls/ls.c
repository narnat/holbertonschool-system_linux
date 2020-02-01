#include "ls.h"

/**
 * main - Entry point for ls
 * @argc: Argument count
 * @argv: Argument vector
 * Return: Return success
*/
int main(int argc, char *argv[])
{
	args_t args;
	opts_t opt = {0};

	args.argc = argc;
	args.argv = argv;
	args.opt = opt;
	args.err = NO_ERR;
	args.inv = -1; /* Combine inv and dir_name */
	args.dir_name = NULL;
	args.n_files = 0;
	args.n_dirs = 0;

	set_opts(&args);
	ls(&args);

	return (args.err);
}

/**
 * ls - custom ls function
 * @args: arguments
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int ls(args_t *args)
{
	int files_i = 0, argv_i;
	size_t width[4] = {0};
	queue_t *dirs = create_queue();
	container_t *files = malloc((args->argc) * sizeof(container_t));

	if (!files)
		return (EXIT_FAILURE);
	for (argv_i = 1; argv_i < args->argc; argv_i++)
	{
		if (args->argv[argv_i][0] == '-' && args->argv[argv_i][1])
			continue;
		if (lstat(args->argv[argv_i], &(files[files_i].sb)) == -1)
		{
			args->err = FILE_ERR, args->inv = argv_i, error(args);
		}
		else if ((files[files_i].sb.st_mode & S_IFMT) == S_IFDIR)
		{
			files[files_i].name = args->argv[argv_i];
			en_queue(dirs, files[files_i]);
		}
		else
		{
			get_info_width(width, files[files_i].sb);
			files[files_i].name = args->argv[argv_i], files_i++;
		}
	}

	if (files_i == 0 && dirs->size == 0 && args->err != FILE_ERR)
	{
		lstat(".", &(files[files_i].sb));
		files[files_i].name = ".";
		en_queue(dirs, files[files_i]);
	}
	args->n_files = files_i, args->n_dirs = dirs->size;
	print_files(args, files, files_i, width);
	print_dirs(args, dirs);
	free(files);

	return (EXIT_SUCCESS);
}

/**
 * read_files - opens dir and reads files
 * @args: global arguments
 * @dir: One directory, which needs to be opened
 * @size: initial size of array
 * @idx: Index of dir for printing
 */
void read_files(args_t *args, node_t *dir, int size, uint idx)
{
	int cur_size = 0, temp;
	DIR *open_dir;
	char buf[BUFSIZ];
	struct dirent *read;
	container_t *files = NULL;
	size_t width[4] = {0};

	size = size * sizeof(container_t);
	files = malloc((size) * sizeof(container_t));
	if (!files)
		exit(EXIT_FAILURE);

	open_dir = opendir(dir->dir.name);
	if (!open_dir)
	{
		args->err = P_ERR, args->dir_name = dir->dir.name, free(files);
		printf("%s", idx == 0 && args->n_files ? "\n" : ""), error(args);
	}
	else
	{
		print_dir_name(args, dir->dir.name, idx);
		while ((read = readdir(open_dir)) != NULL)
		{
			if (cur_size * (int)sizeof(container_t) == size)
			{
				files = _realloc(files, size, size * 2), size *= 2;
			}
			if (!list_hidden(args, read->d_name))
				continue;
			files[cur_size].name = _strdup(read->d_name), temp = _strlen(dir->dir.name);
			if (dir->dir.name[temp - 1] == '/')
				dir->dir.name[temp - 1] = '\0';
			sprintf(buf, "%s/%s", dir->dir.name, read->d_name);
			lstat(buf, &(files[cur_size].sb));
			get_info_width(width, files[cur_size].sb), cur_size++;
		}
		print_files(args, files, cur_size, width);
		closedir(open_dir);
		free_arr(files, cur_size);
	}
}
