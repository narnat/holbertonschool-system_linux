#include "ls.h"

/**
 * get_time - returns time
 * @sb: file info struct
 * Return: correct time
 */
char *get_time(stat_t sb)
{
	char *time = NULL;

	time = ctime(&(sb.st_mtime));
	time[16] = '\0';

	return (&time[4]);
}
