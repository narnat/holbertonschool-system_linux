#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/sysmacros.h>

/**
 * enum err_codes - error codes
 * @NO_ERR: No error code
 * @OPT_ERR: Operation error code
 * @FILE_ERR: No such file or dir error
 * @P_ERR: Permission error
 */
enum err_codes
{
	NO_ERR,
	OPT_ERR,
	FILE_ERR,
	P_ERR
};

/**
 * enum ls_codes - ls exit status codes
 * @LS_OK: exit code 0, normal termination
 * @LS_MINOR: exit code 1, minor problems
 * @LS_SERIOUS: exit code 2, serious problem
 */
enum ls_codes
{
	LS_OK,
	LS_MINOR,
	LS_SERIOUS
};

typedef struct stat stat_t;

/**
 * struct options_s - options struct for ls
 * @show_hidden: -a or -A option
 * @show_cur_and_parent: -a option
 * @put_newline: -l option or -1 option
 * @show_info: -l option
 * @show_nested: -R option
 * @sort_reverse: -r option
 * @sort_size: -S option
 * @sort_time: -t option
 * @print_dir_name: prints names of dirs
 * Description: options struct
 */
typedef struct options_s
{
	unsigned int show_hidden			 :1;
	unsigned int show_cur_and_parent	 :1;
	unsigned int put_newline			 :1;
	unsigned int show_info				 :1;
	unsigned int show_nested			 :1;
	/* Sort options are exclusive to each other */
	unsigned int sort_reverse			 :1;
	unsigned int sort_size				 :1;
	unsigned int sort_time				 :1;

	unsigned int print_dir_name			 :1;
} opts_t;

/**
 * struct arguments_s - Values to be accessed by various functions
 * @argc: counts number of lines
 * @argv: Name of the executable
 * @opt: input line
 * @err: err code
 * @inv: invalid command or option
 * @dir_name: dir name for error
 * @n_files: number of files passed as argument
 * @n_dirs: number of dirs passed as argument
 */
typedef struct arguments_s
{
	int argc;
	char **argv;
	opts_t opt;
	enum err_codes err;
	int inv;
	char *dir_name;
	int n_files;
	int n_dirs;
} args_t;


/**
 * struct container_s - Container struct to store info about files and its name
 * @name: name of the file
 * @sb: info about file
*/
typedef struct container_s
{
	char *name;
	stat_t sb;
} container_t;


/**
 * struct node_s - singly linked list
 * @dir: dir entries
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 */
typedef struct node_s
{
	container_t dir;
	struct node_s *next;
} node_t;

/**
 * struct queue_s - Queue
 * @size: size of queue
 * @first: first node
 * @last: last node
 */
typedef struct queue_s
{
	int size;
	node_t *first;
	node_t *last;
} queue_t;

/* Queue functions functions */
void en_queue(queue_t *queue, container_t dir);
queue_t *create_queue();
void free_queue(queue_t **queue);

int ls(args_t *args);
void error(args_t *args);
void set_opts(args_t *args);
void print_args(args_t *args);
void print_files(args_t *args, container_t *files, int size);
void print_dirs(args_t *args, queue_t *dirs);
void read_files(args_t *args, node_t *dir, int size, uint idx);
void free_arr(container_t *files, int size);
int list_hidden(args_t *args, char *file_name);
void print_dir_name(args_t *args, char *name, uint idx);

void print_info(args_t *args, container_t *file);
char get_file_type(stat_t sb);

char *_strdup(char *str);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char *_strchr(char *s, char c);
int _strlen(char *s);
int _strcmp(char *s1, char *s2);

#endif /* LS_H */
