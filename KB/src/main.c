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

void	print_tokens(t_list *tokens)
{
	t_list *cursor;

	cursor = tokens;
	while (cursor != NULL)
	{
		printf("%s\n", (char *)cursor->content);
		cursor = cursor->next;
	}
}

void	test_env(t_env_vars *env_copy)
{
	t_env_vars *curr;

	curr = env_copy;
	while (curr != NULL)
	{
		printf("key %s\n", curr->key);
		for (int i = 0; curr->values[i] != NULL; i++)
			printf("value: %s\n", curr->values[i]);
		curr = curr->next;
	}
}

/**
 *
 */
int main(int argc, char **argv, char **envp)
{
	char		*input;
	t_list		*tokens;
	t_env_vars	*env_copy;

	env_copy = NULL;
	initialize(argc, argv, &env_copy, envp);
	test_env(env_copy); // eventually remove BTW
	while (1)
	{
		input = listen_input(STDIN_FILENO);
		if (input == NULL)
			break ;
		if (input[0] == '\n')
		{
			free(input);
			continue ;
		}
		tokens = parse_tokens(input);
		print_tokens(tokens);
	}
	return (g_signal);
}
