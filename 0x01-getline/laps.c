#include <limits.h>
#include "laps.h"
#include <string.h>


/**
 * print_listint - prints all elements of a listint_t list
 * @h: pointer to head of list
 * Return: number of nodes
 */
size_t print_listint(const listint_t *h)
{
	const listint_t *current;
	unsigned int n; /* number of nodes */

	current = h;
	n = 0;
	while (current != NULL)
	{
		printf("Car %i [%lu laps]\n", current->n, current->laps);
		current = current->next;
		n++;
	}

	return (n);
}

/**
 * free_listint - frees a listint_t list
 * @head: pointer to list to be freed
 * Return: void
 */
void free_listint(listint_t *head)
{
	listint_t *current;

	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current);
	}
}


/**
 * insert_node - inserts node at correct place
 * @head: head of the linked list
 * @number: data of the new node
 * Return: sorted linked list with a new node
 */
listint_t *insert_node(listint_t **head, int number)
{
	listint_t *node = NULL;
	listint_t *prev = NULL;
	listint_t *next = NULL;

	if (!head)
		return (NULL);

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->n = number;
	node->laps = 0;
	node->next = NULL;
	next = *head;
	printf("Car %i joined the race\n", node->n);

	if (!*head)
		return (node);
	while (prev || next)
	{
		if ((!prev || prev->n <= number) && (!next || next->n > number))
		{
			if (!prev)
				*head = node;
			else
				prev->next = node;
			node->next = next;
		}
		prev = next;
		if (next)
			next = next->next;
	}

	return (*head);
}


/**
 * get_id - get id of the car
 * @head: linked list of cars
 * @id: the id to look for
 * Return: the node with given @id or NULL
 */
listint_t *get_id(listint_t *head, int id)
{
	while (head)
	{
		if (head->n == id)
			return (head);
		head = head->next;
	}

	return (NULL);
}


/**
 * race_state - Tells the state of the race
 * @id: array of cars with unique IDs
 * @size: sizeof @id
 */
void race_state(int *id, size_t size)
{
	static listint_t *list;
	listint_t *tmp;
	size_t i;

	if (!size)
	{
		free_listint(list);
		return;
	}

	if (list == NULL)
	{
		list = insert_node(&list, id[0]);
		list->laps--;
	}

	for (i = 0; i < size; i++)
	{
		tmp = get_id(list, id[i]);
		if (!tmp)
			insert_node(&list, id[i]);
		else
			tmp->laps++;
	}

	printf("Race state:\n");
	print_listint(list);
}
