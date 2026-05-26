/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 12:09:49 by kmonjard          #+#    #+#             */
/*   Updated: 2025/12/24 14:09:50 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

// /**
//  * @brief Gathers input depending on the descriptor configuration environment.
//  * If interactive, it engages raw key interception mode.
//  * Otherwise, it reads raw stream lines cleanly via get_next_line.
//  */
// static char	*get_next_command(t_minishell *data)
// {
// 	char	*raw_line;
// 	char	*trimmed_line;

// 	if (isatty(STDIN_FILENO))
// 	{
// 		init_prompt(data);
// 		return (listen_input(STDIN_FILENO, data));
// 	}
// 	raw_line = get_next_line(STDIN_FILENO);
// 	if (!raw_line)
// 		return (NULL);
// 	trimmed_line = ft_strtrim(raw_line, "\n");
// 	free(raw_line);
// 	return (trimmed_line);
// }

/**
 * @brief Main is main.
 */
int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;

	if (!initialize(argc, argv, envp, &data))
		return (1);
	while (1)
	{
		init_prompt(&data);
		data.input = listen_input(STDIN_FILENO, &data);
		if (!data.input)
			break ;
		if (check_input(data.input))
		{
			free(data.input);
			continue ;
		}
		data.tokens = lexer(data.input);
		if (data.tokens && check_syntax(data.tokens))
		{
			data.cmds = tokens_to_cmds(data.tokens, &data);
			execute(data.cmds, &data);
		}
		cleanup_loop(&data);
	}
	cleanup_shell(&data);
	return (g_signal);
}
