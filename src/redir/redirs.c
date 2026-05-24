/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 17:20:58 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/18 18:18:10 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>

/**
 * @brief Opens the redirect file descriptor for output or append,
 * then duplicates it to STDOUT_FILENO.
 */
static int	handle_output_redirect(t_redir *r, t_minishell *data)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	flags = check_and_set_flags(r, flags);
	fd = open(r->file, flags, 0644);
	if (fd < 0)
		exit_err(1, 1, "open in redirections", data);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

/**
 * @brief Applies redirections of the child
 */
void	apply_redirections_child(t_cmd *cmd, t_minishell *data)
{
	t_redir	*r;
	int		fd;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN || r->type == HEREDOC)
		{
			fd = open(r->file, O_RDONLY);
			if (fd < 0)
				exit_err(1, 1, "open in redirections", data);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
			handle_output_redirect(r, data);
		r = r->next;
	}
}

/**
 * @brief Mallocs a redirect node
 */
t_redir	*new_redir_node(t_token_type type, char *file, int exp)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = file;
	node->expand_heredoc = exp;
	node->next = NULL;
	return (node);
}

/**
 * @brief Adds a redirect node to the back
 */
void	add_redir_back(t_redir **head, t_redir *new_node)
{
	t_redir	*curr;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}

/**
 * @brief Frees the redirects
 */
void	free_redirs(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->file)
			free(head->file);
		free(head);
		head = tmp;
	}
}
