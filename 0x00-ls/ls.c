#include "ls.h"

/**
 * print_args - print args for debugging
 * @args: Arguments
 */
void print_args(args_t *args)
{
    printf("\n\n");
	printf("Printing options:\n");
	printf("\t hidden: %u\n", args->opt.show_hidden);
	printf("\t parent: %u\n", args->opt.show_cur_and_parent);
	printf("\t newline: %u\n", args->opt.put_newline);
	printf("\t info: %u\n", args->opt.show_info);
	printf("\t nested: %u\n", args->opt.show_nested);
	printf("\t reverse: %u\n", args->opt.sort_reverse);
	printf("\t size: %u\n", args->opt.sort_size);
	printf("\t time: %u\n", args->opt.sort_time);
    printf("\n\n");
}


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

	set_opts(&args);
    ls(&args);

	return (args.err);
}

/**
 * set_opts - set options
 * @args: arguments
 * Return: Nothing
 */
void set_opts(args_t *args)
{
	int i, j;

	for (i = 1; i < args->argc; i++)
		if (args->argv[i][0] == '-' && args->argv[i][1])
			for (j = 1; args->argv[i][j]; j++)
				switch (args->argv[i][j])
				{
				case ('1'):
					args->opt.put_newline = 1;
					break;
				case ('a'):
					args->opt.show_hidden = 1;
					args->opt.show_cur_and_parent = 1;
					break;
				case ('A'):
					args->opt.show_hidden = 1;
					break;
				case ('l'):
					args->opt.show_info = 1;
					break;
				case ('r'):
					args->opt.sort_reverse = 1, args->opt.sort_size = 0;
					args->opt.sort_time = 0;
					break;
				case ('S'):
					args->opt.sort_size = 1, args->opt.sort_reverse = 0;
					args->opt.sort_time = 0;
					break;
				case ('t'):
					args->opt.sort_time = 1, args->opt.sort_reverse = 0;
					args->opt.sort_size = 0;
					break;
				case ('R'):
					args->opt.show_nested = 1;
					break;
				default:
					args->err = OPT_ERR;
					args->inv = i, error(args);
                }
}

/**
 * ls - custom ls function
 * @args: arguments
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int ls(args_t *args)
{
    int files_i = 0, argv_i;
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
            files[files_i].name = args->argv[argv_i], files_i++;
        }
    }

    if (files_i == 0 && dirs->size == 0)
    {
        lstat(".", &(files[files_i].sb));
        files[files_i].name = ".";
        en_queue(dirs, files[files_i]);
    }
    if (files_i + dirs->size > 1)
        args->opt.print_dir_name = 1;
    print_files(args, files, files_i);
    print_dirs(args, dirs);

    return (EXIT_SUCCESS);
}


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

    printf("Sorting files\n");
    for (files_i = 0; files_i < size; files_i++)
    {
        printf("%s%s", files[files_i].name, separator);
    }
    free(files);
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

/**
 * read_files - opens dir and reads files
 * @args: global arguments
 * @dir: One directory, which needs to be opened
 * @size: initial size of array
 */
void read_files(args_t *args, node_t *dir, int size)
{
    int cur_size = 0;
    DIR *open_dir;
    char buf[BUFSIZ];
    struct dirent *read;
    container_t *files = malloc((size) * sizeof(container_t));
    /* queue_t *dirs = create_queue(); */
    int temp;

    if (!files)
        exit(EXIT_FAILURE);

    open_dir = opendir(dir->dir.name);

    if (!open_dir)
    {
        args->err = PERM_ERR, args->dir_name = dir->dir.name;
        error(args);
    }
    else
    {
        if (args->opt.print_dir_name)
            printf("%s:\n", dir->dir.name);
        while ((read = readdir(open_dir)) != NULL)
        {
            if (cur_size == size)
            {
                files = _realloc(files, size, size * 2);
                size *= 2;
            }
            files[cur_size].name = _strdup(read->d_name);
            temp = _strlen(dir->dir.name);      /* handles extra '/' at the end of args*/
            if (dir->dir.name[temp - 1] == '/')
                dir->dir.name[temp - 1] = '\0';
            sprintf(buf, "%s/%s", dir->dir.name, read->d_name);
            lstat(buf, &(files[cur_size].sb));

            /* if ((files[cur_size].sb.st_mode & S_IFMT) == S_IFDIR) */
            /* { */
            /*     en_queue(dirs, files[cur_size]); */
            /* } */
            cur_size++;

        }
        print_files(args, files, cur_size);
    }
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
		sprintf(buf,
				"invalid option -- '%c'\nTry"
				" 'ls --help' for more information.\n", args->argv[args->inv][1]);
		fprintf(stderr, "%s", buf);
		exit(LS_SERIOUS);
	}
    if (args->err == FILE_ERR)
    {
        sprintf(buf, "cannot access %s", args->argv[args->inv]);
        perror(buf);
    }
    if (args->err == PERM_ERR)
    {
        sprintf(buf, "cannot open directory %s", args->dir_name);
        perror(buf);
    }
}
