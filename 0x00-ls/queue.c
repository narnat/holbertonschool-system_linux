#include "ls.h"

/**
 * create_queue - Creates an empty queue
 * Return: new queue
*/
queue_t *create_queue()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        exit(EXIT_FAILURE);

    q->first = q->last = NULL;
    q->size = 0;

    return q;
}

/**
 * en_queue - en_queue
 * @queue: queue
 * @dir: directory
 */
void en_queue(queue_t *queue, container_t dir)
{
    node_t *temp = malloc(sizeof(node_t));

    if (!temp)
        exit(EXIT_FAILURE);

    temp->dir = dir;
    temp->next = NULL;
    queue->size++;
    if (queue->last == NULL)
    {
        queue->first = queue->last = temp;
    }
    else
    {
        queue->last->next = temp;
        queue->last = temp;
    }
}


/**
* free_queue - frees a queue
* @q: pointer to head of queue
* Return: Nothing.
*/
void free_queue(queue_t **q)
{
	node_t *prev;
    node_t *head;

    head = (*q)->first;

    if (!head)
		return;
	while (head)
	{
        printf("%s\n", head->dir.name);
        prev = head->next;
        free(head);
		head = prev;
	}

    free(*q);
    *q = NULL;
}
