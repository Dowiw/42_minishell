/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:35:29 by anematol          #+#    #+#             */
/*   Updated: 2025/09/26 19:53:40 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_cmd_paths(char **envp, char **argv, char *cmdpaths[2])
{
	if (access(argv[1], F_OK) != 0 || access(argv[1], R_OK) != 0)
	{
		cmdpaths[0] = NULL;
		acc_error(argv[1], 2);
	}
	else if (!set_cmd_path(envp, argv[2], cmdpaths))
		return (exit(1));
	if (!set_cmd_path(envp, argv[3], cmdpaths + 1))
		return (free(cmdpaths[0]), exit(1));
}

void	exec_child1(char **argv, char **envp, int *pipe_fds, char *cmdpaths[2])
{
	int		in_fd;
	char	**cmd_args;

	free(cmdpaths[1]);
	close(pipe_fds[0]);
	if (*envp == NULL || cmdpaths[0] == NULL)
		return (exit(1));
	in_fd = open(argv[1], O_RDONLY);
	if (in_fd == -1)
		return (free(cmdpaths[0]), close(pipe_fds[1]), exit(1));
	dup2(pipe_fds[1], STDOUT_FILENO);
	close(pipe_fds[1]);
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
	cmd_args = ft_split(argv[2], ' ');
	if (execve(cmdpaths[0], cmd_args, envp) == -1)
		return (free_str_array(cmd_args), free(cmdpaths[0]), exit(1));
}

void	exec_child2(char **argv, char **envp, int *pipe_fds, char *cmdpaths[2])
{
	int		out_fd;
	char	**cmd_args;

	free(cmdpaths[0]);
	close(pipe_fds[1]);
	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd == -1)
		return (free(cmdpaths[1]), close(pipe_fds[0]), exit(1));
	if (*envp == NULL || cmdpaths[1] == NULL)
		return (close(pipe_fds[0]), exit(1));
	dup2(pipe_fds[0], STDIN_FILENO);
	close(pipe_fds[0]);
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	cmd_args = ft_split(argv[3], ' ');
	if (execve(cmdpaths[1], cmd_args, envp) == -1)
		return (free_str_array(cmd_args), free(cmdpaths[1]), exit(1));
}

void	close_pipe_and_wait(int pipe_fds[2])
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	wait(NULL);
	wait(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	int			pipe_fds[2];
	pid_t		pid;
	char		*cmdpaths[2];

	if (argc != 5)
		return (write(2, "4 arguments have to be given\n", 29), 1);
	set_cmd_paths(envp, argv, cmdpaths);
	if (pipe(pipe_fds) == -1)
		return (free(cmdpaths[0]), free(cmdpaths[1]),
			close(pipe_fds[0]), close(pipe_fds[1]), 1);
	pid = fork();
	if (pid == -1)
		return (free(cmdpaths[0]), free(cmdpaths[1]),
			close(pipe_fds[0]), close(pipe_fds[1]), 1);
	if (pid == 0)
		exec_child1(argv, envp, pipe_fds, cmdpaths);
	pid = fork();
	if (pid == -1)
		return (free(cmdpaths[0]), free(cmdpaths[1]),
			close(pipe_fds[0]), close(pipe_fds[1]), 1);
	if (pid == 0)
		exec_child2(argv, envp, pipe_fds, cmdpaths);
	close_pipe_and_wait(pipe_fds);
	return (free(cmdpaths[0]), free(cmdpaths[1]), 0);
}
