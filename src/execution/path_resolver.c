/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:25 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/14 18:15:16 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief Wrapper for cheking the right APPEND or TRUNC flag
 */
int	check_and_set_flags(t_redir *r, int flags)
{
	if (r->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}

static char	*get_path(t_env *path_node, int i, char *cmd)
{
	char	*part_path;
	char	*path;

	part_path = ft_strjoin(path_node->values[i], "/");
	if (!part_path)
		return (print_err(1, 1, "malloc error"), NULL);
	path = ft_strjoin(part_path, cmd);
	if (!path)
		return (free(part_path), print_err(1, 1, "malloc error"), NULL);
	free(part_path);
	return (path);
}

/**
 * @brief Handles paths containing slashes (absolute or relative).
 * Exits cleanly with exact status codes if invalid.
 */
static char	*handle_absolute(char *cmd, t_minishell *data)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		ft_putstr_fd("shelld0n: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		cleanup_shell(data);
		exit(126);
	}
	ft_putstr_fd("shelld0n: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	cleanup_shell(data);
	exit(127);
}

/**
 * @brief Checks the PATH for non-executable but existing files.
 * If found, throws 126. Else, returns to let validate_path throw 127.
 */
static void	check_path_permissions(char *cmd, t_minishell *data, t_env *p_node)
{
	char	*path;
	int		i;

	i = 0;
	while (p_node->values[i])
	{
		path = get_path(p_node, i, cmd);
		if (!path)
		{
			cleanup_shell(data);
			exit(1);
		}
		if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
		{
			ft_putstr_fd("shelld0n: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			free(path);
			cleanup_shell(data);
			exit(126);
		}
		free(path);
		i++;
	}
}

/**
 * @brief Resolves the absolute path of a command.
 * Returns an allocated string with the path, or NULL if not found.
 */
char	*get_cmd_path(char *cmd, t_minishell *data)
{
	t_env	*p_node;
	char	*path;
	int		i;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute(cmd, data));
	p_node = get_env_node(data->processed_env, "PATH");
	if (!p_node || !p_node->values)
		return (NULL);
	i = 0;
	while (p_node->values[i])
	{
		path = get_path(p_node, i, cmd);
		if (!path)
			return (cleanup_shell(data), exit(1), NULL);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	check_path_permissions(cmd, data, p_node);
	return (NULL);
}
