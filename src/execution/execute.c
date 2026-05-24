/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:20 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 14:35:11 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

/**
 * @brief Parent thread util.
 *
 * Waits for every child. It loops forever until waitpid returns -1
 * meaning, no more child process is there.
 *
 * If the status child is exited properly through return or exit() then
 * grab the signal and save it to g_signal.
 *
 * Else if the child exited using signals, check what signal made it exit
 * Then do 128 + [SIGNUM] to g_signal.
 */
void	wait_all_children(pid_t last_pid)
{
	int	status;

	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			g_signal = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_signal = 128 + WTERMSIG(status);
	}
	while (waitpid(-1, &status, 0) > 0)
		;
}

/**
 * @brief Parent function.
 */
void	run_parent(t_cmd *curr, int *prev_fd, int fd[2])
{
	if ((*prev_fd) != -1)
		close((*prev_fd));
	if (curr->next)
	{
		close(fd[1]);
		(*prev_fd) = fd[0];
	}
	else
		*prev_fd = -1;
}

/**
 * @brief Checks if a command is a built-in AND has absolutely no pipes.
 */
static int	is_standalone_builtin(t_cmd *head, t_cmd *curr)
{
	char	*cmd;

	if (!curr || !curr->args || !curr->args[0])
		return (0);
	cmd = curr->args[0];
	if (head == curr && !curr->next)
	{
		if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "exit") == 0)
			return (1);
		if (ft_strcmp(cmd, "export") == 0 && curr->args[1])
			return (1);
	}
	return (0);
}

/**
 * @brief Handles the descriptor creation and process spawning.
 */
static void	fork_child(t_cmd *cmds, t_minishell *data, int *prev, int *last_p)
{
	int	fd[2];

	if (cmds->next)
		pipe(fd);
	data->pid = fork();
	if (data->pid == 0)
		run_child(cmds, data, *prev, fd);
	else
	{
		*last_p = data->pid;
		run_parent(cmds, prev, fd);
	}
}

/**
 * @brief Abstract. Execution code.
 *
 * Sets prev_fd to -1. fd for piping. Loops through every command.
 * It checks if there is a new command before setting a pipe up.
 * Then it creates a child process to handle the pipes.
 *
 * For a piped process:
 *
 * - The child process assigns its STOUT to the write of the pipe.
 *
 * - The next child (if it exists) assigns the read of the pipe to its STDIN.
 *
 * The parent manages the pipes and ensures that the prev_fd (read) is
 * available for the next process to assign to its STDIN.
 *
 * More explanations on the redirections inside run_child.
 */
void	execute(t_cmd *cmds, t_minishell *data)
{
	int		prev_fd;
	pid_t	last_pid;
	t_cmd	*head;

	prev_fd = -1;
	last_pid = -1;
	head = cmds;
	if (!cmds)
		return ;
	if (!prep_all_heredocs(cmds, data))
		return ;
	while (cmds)
	{
		if (is_standalone_builtin(head, cmds))
			run_parent_inbuilt(cmds, data);
		else
			fork_child(cmds, data, &prev_fd, &last_pid);
		cmds = cmds->next;
	}
	wait_all_children(last_pid);
	unlink_heredocs(head);
}
