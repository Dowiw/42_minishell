/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:02:59 by anematol          #+#    #+#             */
/*   Updated: 2025/09/26 19:50:26 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//cmd_exists having the value 2 means it is the input file
void	acc_error(char *full_cmd, int cmd_exists)
{
	char	end_char;

	end_char = ' ';
	if (cmd_exists == 2)
	{
		if (access(full_cmd, F_OK) == 0)
			write(2, "permission denied: ", 19);
		else
			write(2, "no such file or directory: ", 27);
		end_char = '\0';
	}
	else if (cmd_exists == 1)
		write(2, "permission denied: ", 19);
	else if (strchr(full_cmd, '/'))
		write(2, "no such file or directory: ", 27);
	else
		write(2, "command not found: ", 19);
	while (*full_cmd && *full_cmd != end_char)
	{
		write(2, full_cmd, 1);
		full_cmd++;
	}
	write(2, "\n", 1);
}

char	*get_first_word(char *str, char delimiter)
{
	char	*word;

	while (*str && *str == delimiter)
		str++;
	word = ft_split_dup(str, delimiter);
	if (!word)
		return (NULL);
	return (word);
}

char	*find_cmd_path(char *full_cmd, char **cmd_args, char **paths)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths && paths[i] != NULL)
	{
		cmd_path = ft_strmerge_with_slash(paths[i], cmd_args[0]);
		if (!cmd_path)
			return (free_str_arrays(paths, cmd_args), exit(1), NULL);
		if (access(cmd_path, F_OK) == 0)
		{
			if (access(cmd_path, X_OK) == 0)
				return (free_str_arrays(paths, cmd_args), cmd_path);
			acc_error(full_cmd, 1);
			return (free_str_arrays(paths, cmd_args), NULL);
		}
		free(cmd_path);
		i++;
	}
	acc_error(full_cmd, 0);
	return (free_str_arrays(paths, cmd_args), NULL);
}

int	check_local_access(char **cmd_p, char *full_cmd, char *path_variable)
{
	char	*temp;

	if (!path_variable)
	{
		temp = ft_strmerge_with_slash(".", *cmd_p);
		free(*cmd_p);
		*cmd_p = temp;
	}
	if (access(*cmd_p, F_OK) == 0)
	{
		if (access(*cmd_p, X_OK) == 0)
			return (1);
		return (acc_error(full_cmd, 1), 0);
	}
	return (0);
}

//return value is 0 if memory allocation fails, otherwise it's 1
//c in full_c (and c_args) refers to command (cmd)
int	set_cmd_path(char **envp, char *full_c, char **cmdpath)
{
	char	*cmd;
	char	**c_args;
	char	**paths;
	int		i;

	i = 0;
	while (envp && envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i])
		paths = ft_split(envp[i] + 5, ':');
	else
		paths = NULL;
	c_args = ft_split(full_c, ' ');
	cmd = get_first_word(full_c, ' ');
	if (!envp || !c_args || !cmd || (!paths && envp[i]))
		return (free_str_arrays(c_args, paths), free(cmd), 0);
	if (!ft_strchr(cmd, '/') && envp && envp[i])
		return (free(cmd), *cmdpath = find_cmd_path(full_c, c_args, paths), 1);
	else if (check_local_access(&cmd, full_c, envp[i]))
		return (free_str_arrays (c_args, paths), *cmdpath = cmd, 1);
	else if (access(cmd, F_OK) == 0)
		return (free(cmd), free_str_arrays(c_args, paths), *cmdpath = NULL, 1);
	else
		return (free(cmd), free_str_arrays(c_args, paths),
			acc_error(full_c, 0), *cmdpath = NULL, 1);
}
