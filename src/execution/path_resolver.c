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
 * @brief Handles the absolute path.
 */
char	*handle_absolute(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

void	path_error(char *cmd, t_minishell *data, t_env *path_node)
{
	char	*path;
	int		i;

	i = 0;
	while (path_node->values[i])
	{
		path = get_path(path_node, i, cmd);
		if (!path)
			return (cleanup_shell(data), exit(1));
		if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
		{
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			free(path);
			return ;
		}
		free(path);
		i++;
	}
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (ft_strchr((const char *)cmd, '/'))
		ft_putstr_fd(": no such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

/**
 * @brief Resolves the absolute path of a command.
 * Returns an allocated string with the path, or NULL if not found.
 */
char	*get_cmd_path(char *cmd, t_minishell *data)
{
	t_env		*path_node;
	char		*path;
	int			i;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	path = handle_absolute(cmd);
	if (path)
		return (path);
	path_node = get_env_node(data->processed_env, "PATH");
	if (!path_node || !path_node->values)
		return (NULL);
	i = 0;
	while (path_node->values[i])
	{
		path = get_path(path_node, i, cmd);
		if (!path)
			return (cleanup_shell(data), exit(1), NULL);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (path_error(cmd, data, path_node), NULL);
}
