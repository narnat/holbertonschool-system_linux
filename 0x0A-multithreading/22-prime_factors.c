#include "list.h"
#include "multithreading.h"
#include <pthread.h>


task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	task = calloc(1, sizeof(*task));
	if (!task)
		return (NULL);
	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	if (pthread_mutex_init(&task->lock, NULL))
	{
		free(task);
		return (NULL);
	}
	return (task);
}

void destroy_task(task_t *task)
{
	list_destroy(task->result, free);
	pthread_mutex_destroy(&task->lock);
	free(task);
}

void *exec_tasks(list_t const *tasks)
{
	node_t *root;
	task_t *task;
	size_t count = 0;

	root = tasks->head;
	while (root)
	{
		task = root->content;
		if (pthread_mutex_trylock(&task->lock) == 0)
		{
			if (task->status == PENDING)
			{
				task->status = STARTED;
				tprintf("[%02lu] Started\n", count);
				task->result = task->entry(task->param);
				tprintf("[%02lu] Success\n", count);
				task->status = SUCCESS;
			}
			pthread_mutex_unlock(&task->lock);
		}
		root = root->next;
		++count;
	}
	return (NULL);
}
