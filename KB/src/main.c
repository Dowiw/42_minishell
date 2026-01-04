/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 12:09:49 by kmonjard          #+#    #+#             */
/*   Updated: 2025/12/24 14:09:50 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *
 */
int main(int argc, char **argv, char **envp)
{
	char		*input;
	t_list		*tokens;
	t_env_vars	*env_copy;

	initialize(argc, argv, &env_copy, envp);
	while (1)
	{
		input = parse_input(STDIN_FILENO);
		if (input == NULL)
			break ;
		if (input[0] == '\n') // for an empty input
		{
			free(input);
			continue;
		}
		tokens = parse_tokens(input);
		perform_tokens(tokens);
		print_tokens(tokens);
		free_tokens(tokens);
		free(input);
	}
	printf("exit\n");
	return (0);
}
