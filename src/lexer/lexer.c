/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:58 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:11:44 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles redirects that have two characters.
 *
 * @returns -1 for errors, string size if not
 */
static int	handle_double_ops(char *input, int i, t_token **list)
{
	t_token	*new;

	if (input[i] == '<' && input[i + 1] == '<')
	{
		new = new_token(ft_strdup("<<"), HEREDOC);
		if (!new)
			return (-1);
		add_token_back(list, new);
		return (2);
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		new = new_token(ft_strdup(">>"), APPEND);
		if (!new)
			return (-1);
		add_token_back(list, new);
		return (2);
	}
	return (0);
}

/**
 * @brief Handle quotes and double quotes and pipes.
 * Returns how many characters it consumed (1 or 2)
 */
static int	handle_operator(char *input, int i, t_token **list)
{
	t_token	*new;

	if ((input[i] == '<' && input[i + 1] == '<')
		|| ((input[i] == '>' && input[i + 1] == '>')))
		return (handle_double_ops(input, i, list));
	else if (input[i] == '<')
	{
		new = new_token(ft_strdup("<"), REDIR_IN);
		if (!new)
			return (-1);
		add_token_back(list, new);
		return (1);
	}
	else if (input[i] == '>')
	{
		new = new_token(ft_strdup(">"), REDIR_OUT);
		if (!new)
			return (-1);
		add_token_back(list, new);
		return (1);
	}
	else if (input[i] == '|')
		return (handle_pipe_token(list));
	return (0);
}

/**
 * @brief Initializes the token of a word and appends it on the list.
 */
static ssize_t	init_token(char *in, char **word, ssize_t *i, t_token **list)
{
	ssize_t	len;
	t_token	*new;

	len = 0;
	len = get_token_len(&in[(*i)]);
	(*word) = ft_substr(in, (*i), len);
	if (!(*word))
		return (-1);
	new = new_token((*word), WORD);
	if (!new)
		return (-1);
	add_token_back(&(*list), new);
	return (len);
}

/**
 * @brief Handles lexing
 */
static int	handle_lexing(ssize_t *i, t_token **list, char **word, char **input)
{
	ssize_t	temp;

	temp = 0;
	if ((*input)[(*i)] == '|' || (*input)[(*i)] == '<' || (*input)[(*i)] == '>')
	{
		temp = handle_operator((*input), (*i), list);
		if (temp < 0)
			return (0);
		else
			(*i) += temp;
	}
	else
	{
		temp = init_token((*input), word, i, list);
		if (temp < 0)
			return (0);
		else
			(*i) += temp;
	}
	return (1);
}

/**
 * @brief Parse the input into tokens for processing.
 */
t_token	*lexer(char *input)
{
	t_token	*list;
	ssize_t	i;
	char	*word;

	if (!input)
		return (NULL);
	list = NULL;
	i = 0;
	word = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (!handle_lexing(&i, &list, &word, &input))
			return (NULL);
	}
	return (list);
}
