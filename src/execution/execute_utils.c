/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:03 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/20 14:05:07 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Unlink heredocs and return to ROM
 */
void	unlink_heredocs(t_cmd *cmds)
{
	t_cmd	*c;
	t_redir	*r;

	c = cmds;
	while (c)
	{
		r = c->redirs;
		while (r)
		{
			if (r->type == HEREDOC && r->file)
				unlink(r->file);
			r = r->next;
		}
		c = c->next;
	}
}

//if a "parent" inbuilt command gets detected, the function
// will run the command and return 1
// a "parent" inbuilt command does not output to stdout
//will also set cmd = cmd->next if an inbuilt command will be executed
int	run_parent_inbuilt(t_cmd *cmd, t_minishell *data)
{
	int	argc;

	argc = 0;
	while (cmd->args[argc])
		argc++;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data->processed_env, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(&(data->processed_env), argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(data->processed_env, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(), 1);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data->processed_env, argc), 1);
	return (0);
}
