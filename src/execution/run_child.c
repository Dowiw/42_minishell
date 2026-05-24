/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 17:21:34 by anematol          #+#    #+#             */
/*   Updated: 2026/05/19 17:50:22 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Validates if the command exists and is not a directory.
 * Handles exits 127 (not found) and 126 (directory).
 */
static char	*validate_path(char *cmd_name, t_minishell *data)
{
	char		*path;
	struct stat	path_stat;

	path = get_cmd_path(cmd_name, data);
	if (!path)
		exit_err(0, 127, NULL, data);
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		free(path);
		exit_err(0, 126, NULL, data);
	}
	return (path);
}

/**
 * @brief Wrapper for inbuilt child functions
 */
static int	run_child_inbuilt(t_cmd *cmd, t_minishell *data)
{
	int	argc;

	argc = 0;
	while (cmd->args[argc])
		argc++;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(), 1);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data->processed_env, argc), 1);
	if (ft_strcmp(cmd->args[0], "export" ) == 0)
		return (ft_export(data->processed_env, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data->processed_env, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data, argc, cmd->args), 1);
	return (0);
}

/**
 * @brief Child process.
 *
 * The first lines of checking prev_fd and cmd->next are handling pipes.
 *
 * For a redirected process:
 *
 * - When an infile is detected, it opens a file of that string name and
 * it takes its STDIN to be replaced by the input incoming from the in file's fd.
 * Closes that once its done.
 *
 * - When an outfile is detected, it creates a file and uses a ternary to
 * determine whether to append or truncate. Passes 0644 as the default file
 * permissions. Then, it replaces its STDOUT with the out file's fd.
 *
 * In execve, any memory allocated once execve finishes frees the memory.
 * Therefore, the fresh_env is always freed.
 */
void	run_child(t_cmd *cmd, t_minishell *data, int prev_fd, int fd[2])
{
	char	**fresh_env;
	char	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	handle_pipes(cmd, &prev_fd, fd);
	apply_redirections_child(cmd, data);
	if (!cmd->args[0])
		exit(0);
	if (run_child_inbuilt(cmd, data))
	{
		cleanup_shell(data);
		exit(0);
	}
	cmd_path = validate_path(cmd->args[0], data);
	fresh_env = convert_env_to_array(data->processed_env);
	execve(cmd_path, cmd->args, fresh_env);
	perror("execve");
	free_str_arrays(fresh_env);
	free(cmd_path);
	cleanup_shell(data);
	exit(1);
}
