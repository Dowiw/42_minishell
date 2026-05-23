/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:05:51 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:12:13 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

/**
 * @brief Appends a newly allocated environment variable node.
 * Updated to use a double pointer to securely update the list head.
 * Norminette Safe: 20 lines inside body.
 */
int	add_env_var(t_env **copy, char *key, char *value)
{
	t_env	*curr;
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (print_err(0, 1, "env malloc"), 0);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
		return (print_err(0, 1, "env malloc"), free(new_node), 0);
	if (!value)
		new_node->values = NULL;
	else
	{
		new_node->values = ft_split(value, ':');
		if (!new_node->values)
			return (print_err(0, 1, "env alloc"), free(new_node->key),
				free(new_node), 0);
	}
	new_node->next = NULL;
	if (*copy == NULL)
		return (*copy = new_node, 1);
	curr = *copy;
	while (curr->next != NULL)
		curr = curr->next;
	return (curr->next = new_node, 1);
}
