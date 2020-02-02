#include "ls.h"

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
					args->opt.put_newline = 1;
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
 * get_file_permissions - gets file permissions
 * @sb: file info struct
 * @perms: permissions char array
 */
void get_file_permissions(stat_t sb, char *perms)
{
	if (sb.st_mode & S_IRUSR)
		perms[0] = 'r';
	if (sb.st_mode & S_IWUSR)
		perms[1] = 'w';
	if (sb.st_mode & S_IXUSR)
		perms[2] = 'x';
	if (sb.st_mode & S_IRGRP)
		perms[3] = 'r';
	if (sb.st_mode & S_IWGRP)
		perms[4] = 'w';
	if (sb.st_mode & S_IXGRP)
		perms[5] = 'x';
	if (sb.st_mode & S_IROTH)
		perms[6] = 'r';
	if (sb.st_mode & S_IWOTH)
		perms[7] = 'w';
	if (sb.st_mode & S_IXOTH)
		perms[8] = 'x';
}

/**
 * print_info - -l option of ls
 * @args: args struct with options
 * @file: file info
 * @width: width array
 */
void print_info(args_t *args, container_t *file, size_t *width)
{
	char permissions[11] = "----------\0";
	struct passwd *uid = getpwuid(file->sb.st_uid);
	struct group *gid = getgrgid(file->sb.st_gid);

	if (!args->opt.show_info)
		return;
	permissions[0] = get_file_type(file->sb);
	get_file_permissions(file->sb, &permissions[1]);
	printf("%s", permissions);
	printf(" %-*ld", (int)width[0], (long) file->sb.st_nlink);

	if (uid)
		printf(" %-*s", (int)width[1], uid->pw_name);
	else
	{
		printf(" %*u", (int)width[1], file->sb.st_uid);
	}

	if (gid)
		printf(" %-*s", (int)width[2], gid->gr_name);
	else
	{
		printf(" %*u", (int)width[2], file->sb.st_gid);
	}

	printf(" %*ld", (int)width[3], (long) file->sb.st_size);
	printf(" %s ", get_time(file->sb));
}

/**
 * get_file_type - returns character with 1st letter of type name
 * @sb: file info struct
 * Return: character with file type, first letter
 */
char get_file_type(stat_t sb)
{
	switch (sb.st_mode & S_IFMT)
	{
	case S_IFBLK:
		return ('b');
	case S_IFCHR:
		return ('c');
	case S_IFDIR:
		return ('d');
	case S_IFIFO:
		return ('f');
	case S_IFLNK:
		return ('l');
	case S_IFREG:
		return ('-');
	case S_IFSOCK:
		return ('s');
	default:
		return ('u');
	}
}

/**
 * list_hidden - function for -a and -A options
 * @args: - args, where options are stored
 * @file_name: file name
 * Return: 1 if show it, 0 hide it
 */
int list_hidden(args_t *args, char *file_name)
{
	if (!args->opt.show_cur_and_parent)
	{
		if (!_strcmp(file_name, ".") || !_strcmp(file_name, ".."))
			return (0);
	}
	if (!args->opt.show_hidden)
	{
		if (file_name[0] == '.')
			return (0);
	}
	return (1);
}
