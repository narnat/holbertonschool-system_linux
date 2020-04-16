#include "rb_trees.h"

/**
 * rb_tree_node - create a new RB node
 * @parent: parent pointer
 * @value: value of node
 * @color: color of the node
 * Return: pointer to the new node, or NULL on failure
 */
rb_tree_t *rb_tree_node(rb_tree_t *parent, int value, rb_color_t color)
{
	rb_tree_t *new_node;

	new_node = malloc(sizeof(new_node));
	if (!new_node)
		return (NULL);

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = parent;
	new_node->n = value;
	new_node->color = color;

	return (new_node);
}
