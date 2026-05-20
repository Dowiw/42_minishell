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

/**
 * @brief Main is main.
 */
int	main(int argc, char **argv, char **envp)
{
	t_minishell	data;

	initialize(argc, argv, envp, &data);
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
