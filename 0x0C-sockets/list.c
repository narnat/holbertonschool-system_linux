#include "data_structures.h"

/**
 * delete_nodeint_at_index - deletes nth element
 * @head: a pointer to the head
 * @index: index of a node to be removed
 * Return: Return 1 if succes or -1 when it fails
 */
int delete_nodeint_at_index(todo_list **head, unsigned int index)
{
	unsigned int i;
	todo_list *current;
	todo_list *forward;

	if (*head == NULL)
		return (-1);
	current = *head;
	if (index == 0)
	{
		*head = current->next;
		free(current);
		return (1);
	}
	for (i = 0; i < index - 1; i++)
	{
		if (current->next == NULL)
			return (-1);
		current = current->next;
	}
	forward = *head;
	if (forward->next == NULL)
		return (-1);
	forward = current->next;
	current->next = forward->next;
	free(forward);
	return (1);
}

/**
 * find_by_id - find a todo by id
 * @queue: queue of todos
 * @id: id of a todo
 * Return: todo with matching @id or NULL
 */
todo_list *find_by_id(queue_t *queue, size_t id)
{
	todo_list *node;

	if (!queue || !queue->size)
		return (NULL);
	node = queue->first;
	while (node && node->id != id)
		node = node->next;
	return (node);
}
