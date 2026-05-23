/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:17 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:18:02 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Fills in commands with redirects.
 */
static void	fill_redirs(t_token **tok, t_cmd **cmd, t_minishell *data)
{
	t_token_type	type;
	char			*file;

	if ((*tok)->type == REDIR_OUT || (*tok)->type == APPEND
		|| (*tok)->type == REDIR_IN)
	{
		type = (*tok)->type;
		(*tok) = (*tok)->next;
		if (*tok)
		{
			file = expansion((*tok)->value, data->processed_env);
			add_redir_back(&(*cmd)->redirs, new_redir_node(type, file, 0));
		}
	}
}

/**
 * @brief Fills in commands, heredocs and calls redir filler.
 *
 * @return 0 on error, 1 on good
 */
static int	fill_cmd(t_token **tok, t_cmd **curr, int *i, t_minishell *data)
{
	t_redir	*new;
	char	*delim;
	int		exp;

	if ((*tok)->type == WORD)
	{
		(*curr)->args[(*i)] = expansion((*tok)->value, data->processed_env);
		if (!(*curr)->args[(*i)])
			return (0);
		(*i)++;
	}
	else if ((*tok)->type == HEREDOC)
	{
		(*curr)->heredoc = 1;
		exp = !has_quotes((*tok)->next->value);
		delim = strip_quotes_only((*tok)->next->value);
		new = new_redir_node(HEREDOC, delim, exp);
		if (!new)
			return (0);
		add_redir_back(&(*curr)->redirs, new);
		(*tok) = (*tok)->next;
	}
	else
		fill_redirs(tok, curr, data);
	return (1);
}

/**
 * @brief Tiny helper to allocate a new command and attach it to the list.
 */
static t_cmd	*start_new_cmd(t_token *tok, t_cmd **head, int *i)
{
	t_cmd	*new_cmd;

	new_cmd = init_cmd(tok);
	if (new_cmd)
	{
		add_cmd_back(head, new_cmd);
		*i = 0;
	}
	return (new_cmd);
}

/**
 * @brief Converts each token to command structure for easier parsing later on.
 * Along with their args.
 */
t_cmd	*tokens_to_cmds(t_token *tokens, t_minishell *data)
{
	int		i;
	t_cmd	*head;
	t_cmd	*curr;

	i = 0;
	head = NULL;
	curr = NULL;
	while (tokens)
	{
		if (!curr)
			curr = start_new_cmd(tokens, &head, &i);
		if (!curr)
			return (NULL);
		if (tokens->type == PIPE)
			curr = NULL;
		else if (!fill_cmd(&tokens, &curr, &i, data))
			return (NULL);
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}
