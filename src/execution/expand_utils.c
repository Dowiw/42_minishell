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

static int	count_split_elements(char *str)
{
	char	**matrix;
	int		count;

	if (!str)
		return (0);
	matrix = ft_split(str, ' ');
	if (!matrix)
		return (0);
	count = 0;
	while (matrix[count])
		count++;
	free_str_arrays(matrix);
	return (count);
}

int	count_args_with_splits(t_token *tok, t_minishell *data)
{
	int		count;
	char	*exp;

	count = 0;
	while (tok && tok->type != PIPE)
	{
		if (tok->type == WORD)
		{
			exp = expansion(tok->value, data->processed_env);
			if (exp && exp[0] == '\0' && !has_quotes(tok->value))
				count += 0;
			else if (exp && !has_quotes(tok->value) && (ft_strchr(exp, ' ')
					|| ft_strchr(exp, '\t')))
				count += count_split_elements(exp);
			else
				count++;
			free(exp);
		}
		tok = tok->next;
	}
	return (count);
}

/**
 * @brief Distributes split strings into the args matrix array.
 */
static int	distribute_words(char **matrix, t_cmd **curr, int *i)
{
	int	idx;

	idx = 0;
	while (matrix[idx])
	{
		(*curr)->args[(*i)] = ft_strdup(matrix[idx]);
		if (!(*curr)->args[(*i)])
		{
			while ((*curr)->args[--(*i)])
				free((*curr)->args[(*i)]);
			return (0);
		}
		(*i)++;
		idx++;
	}
	free_str_arrays(matrix);
	return (1);
}

/**
 * @brief Handles expansion, filters unquoted empty vars,
 * and applies word splitting rules to whitespace sequences.
 *
 * @return 0 for err, 1 for gut
 */
int	fill_word(t_token **tok, t_cmd **curr, int *i, t_minishell *data)
{
	char	*expanded;
	char	**matrix;

	expanded = expansion((*tok)->value, data->processed_env);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0' && !has_quotes((*tok)->value))
	{
		free(expanded);
		return (1);
	}
	if (!has_quotes((*tok)->value) && (ft_strchr(expanded, ' ')
			|| ft_strchr(expanded, '\t')))
	{
		matrix = ft_split(expanded, ' ');
		free(expanded);
		if (!matrix)
			return (0);
		if (!distribute_words(matrix, curr, i))
			return (0);
	}
	else
		(*curr)->args[(*i)++] = expanded;
	return (1);
}
