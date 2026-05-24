/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:49:03 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:12:24 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Helper to determine how many words an unquoted expansion produces.
 */
static int	count_split_words(char *expanded)
{
	char	**matrix;
	int		count;

	if (!expanded)
		return (0);
	matrix = ft_split(expanded, ' ');
	if (!matrix)
		return (0);
	count = 0;
	while (matrix[count])
		count++;
	free_str_arrays(matrix);
	return (count);
}

/**
 * @brief Counts tokens until the next pipe, factoring in word splitting.
 */
int	count_args(t_token *curr, t_minishell *data)
{
	int		count;
	char	*exp;

	count = 0;
	while (curr && curr->type != PIPE)
	{
		if (curr->type == WORD)
		{
			exp = expansion(curr->value, data->processed_env);
			if (!exp)
				return (-1);
			if (exp && exp[0] == '\0' && !has_quotes(curr->value))
				count += 0;
			else if (exp && !has_quotes(curr->value)
				&& (ft_strchr(exp, ' ') || ft_strchr(exp, '\t')))
				count += count_split_words(exp);
			else
				count++;
			free(exp);
		}
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Allocates a new command node and its arguments array safely.
 */
t_cmd	*init_cmd(t_token *curr_start, t_minishell *data)
{
	t_cmd	*cmd;
	int		max_args;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (print_err(1, 1, "malloc in init_cmd"), NULL);
	max_args = count_args(curr_start, data);
	if (max_args < 0)
		return (free(cmd), print_err(1, 1, "fail in init_cmd"), NULL);
	cmd->args = ft_calloc(max_args + 1, sizeof(char *));
	if (!cmd->args)
		return (free(cmd), print_err(1, 1, "malloc in init_cmd"), NULL);
	return (cmd);
}

/**
 * @brief Appends a command node to the back of the list.
 */
void	add_cmd_back(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!list || !new_cmd)
		return ;
	if (*list == NULL)
	{
		*list = new_cmd;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_cmd;
}
