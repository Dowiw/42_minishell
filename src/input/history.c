/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:44 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/14 19:06:56 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Reset the index to the count every string entry.
 * Free the buffer if it exists.
 */
void	reset_history(t_history *history)
{
	history->history_index = history->history_count;
	if (history->buffer)
	{
		free(history->buffer);
		history->buffer = NULL;
	}
}

/**
 * @brief Initialize the new history string.
 *
 * @returns 1 for errors, 0 for success
 */
static int	init_new_history(t_history **his, long *c, char **new, char **in)
{
	if ((*his)->history)
	{
		while ((*his)->history[(*c)])
		{
			new[(*c)] = (*his)->history[(*c)];
			(*c)++;
		}
	}
	new[(*c)] = ft_strdup((*in));
	if (!new[(*c)])
		return (1);
	new[(*c) + 1] = NULL;
	return (0);
}

/**
 * @brief Appends a new command string to the shell's history array.
 *
 * If the input is NULL or the inputted string is "", it returns
 */
void	append_to_history(char **input, t_history *history)
{
	long	count;
	char	**new_history;

	if (!(*input) || (*input)[0] == '\0')
		return ;
	count = 0;
	if (history->history)
	{
		while (history->history[count])
			count++;
	}
	history->history_count = count;
	if (count > 0 && ft_strcmp(history->history[count - 1], (*input)) == 0)
		return ;
	new_history = malloc(sizeof(char *) * (count + 2));
	if (!new_history)
		return (print_err(1, 1, "failed to append history"));
	count = 0;
	if (init_new_history(&history, &count, new_history, input))
		return (free(new_history), print_err(1, 1, "failed to add history"));
	if (history->history)
		free(history->history);
	history->history_count = count + 1;
	history->history = new_history;
}
