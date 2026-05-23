/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:04:11 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/14 16:36:11 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief A subtle check if SHLVL or PWD is not found.
 * This means the shell is level one.
 *
 * @returns 1 for error, 0 for sucess
 */
static int	not_found_env(t_env *env_copy)
{
	t_env		*curr;
	int			bool_shlvl;
	int			bool_pwd;

	bool_shlvl = 0;
	bool_pwd = 0;
	curr = env_copy;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, "SHLVL") == 0)
			bool_shlvl = 1;
		else if (ft_strcmp(curr->key, "PWD") == 0)
			bool_pwd = 1;
		curr = curr->next;
	}
	if (bool_shlvl == 0)
	{
		if (!add_env_var(env_copy, "SHLVL", "1"))
			return (1);

	}if (bool_pwd == 0)
	{
		if (!add_env_var(env_copy, "PWD", getcwd(NULL, 0)))
			return (1);
	}
	return (0);
}

/**
 * @brief Modify built in shell environment variables.
 */
static int	update_variables(t_env *env_copy)
{
	t_env		*curr;

	curr = env_copy;
	while (curr != NULL)
	{
		if (modify_variables(&curr))
			return (1);
		curr = curr->next;
	}
	if (not_found_env(env_copy))
		return (1);
	return (0);
}

/**
 * @brief Fills up the current env structure.
 */
static int	fill_env(t_env **env_copy, char **envp, t_env **cur)
{
	size_t	i;

	i = 0;
	while ((*envp)[i] != '=')
		i++;
	(*cur)->next= NULL;
	(*cur)->key = malloc(sizeof(char) * (i + 1));
	if (!(*cur)->key)
		return (free((*cur)), free_env(*env_copy), 1);
	ft_strncpy((*cur)->key, *envp, i);
	(*cur)->key[i] = '\0';
	(*cur)->values = ft_split(*envp + i + 1, ':');
	if (!(*cur)->values)
		return (free((*cur)->key), free((*cur)), free_env(*env_copy), 1);
	return (0);
}

/**
 * @brief Initialize a copy of the environment.
 */
static int	initialize_env(t_env **env_copy, char **envp)
{
	t_env	*curr;
	t_env	*prev;

	curr = NULL;
	prev = NULL;
	while (envp && *envp != NULL)
	{
		curr = malloc(sizeof(t_env));
		if (!curr)
			return (free_env((*env_copy)), 1);
		if (fill_env(env_copy, envp, &curr))
			return (free_env((*env_copy)), 1);
		if ((*env_copy) == NULL)
			(*env_copy) = curr;
		else
			prev->next = curr;
		prev = curr;
		envp++;
	}
	if (update_variables((*env_copy)))
		return (free_env((*env_copy)), 1);
	return (0);
}

/**
 * @brief Initialize the environment & signal handlers.
 * Exits failure if too many arguments.
 */
int	initialize(int argc, char **argv, char **envp, t_minishell *data)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("shelld0n[1]: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	ft_bzero(data, sizeof(t_minishell));
	if (initialize_env(&data->processed_env, envp))
		return (print_err(1, 1, "failed to init env"), 1);
	setup_signals();
	return (0);
}
