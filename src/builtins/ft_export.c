/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:07:35 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:25:31 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

//returns a malloced string
//returns NULL on malloc error
char	*get_key(const char *key_value)
{
	int		i;
	char	*key;

	i = 0;
	while (key_value[i] && key_value[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (print_err(1, 1, "failed to malloc key"), NULL);
	ft_strncpy(key, key_value, i);
	key[i] = '\0';
	return (key);
}

//returns a pointer to the start of values
//so basically a pointer to the firts character after "="
//if there is no "=", NULL is returned
static const char	*get_values_pos(const char *key_value)
{
	int	i;

	i = 0;
	while (key_value[i] && key_value[i] != '=')
		i++;
	if (key_value[i] == '\0')
		return (key_value + i);
	else
		return (key_value + i + 1);
}

/**
 * @brief Prints variables when there are no args
 */
static void	print_export(t_env *env)
{
	int	i;

	while (env)
	{
		ft_printf("declare -x %s", env->key);
		i = 0;
		if (env->values)
		{
			ft_printf("=\"");
			while ((env->values)[i])
			{
				if (i > 0)
					ft_printf(":");
				ft_printf("%s", (env->values)[i]);
				i++;
			}
			ft_printf("\"");
		}
		ft_printf("\n");
		env = env->next;
	}
}

/**
 * @brief Export
 */
static int	export_variable(t_env *env, char **argv, int i)
{
	char	*key;
	t_env	*env_node;

	ft_printf("export : %s\n", argv[i]);
	key = get_key(argv[(i)]);
	if (!key)
		return (print_err(1, 1, "malloc in export"), 0);
	if (key[0] == '\0' || !ft_isalpha(key[0]))
	{
		ft_printf("shelld0n[1]: export: `%s': not a valid identifier\n",
			argv[i]);
		return (free(key), g_signal = 1, 0);
	}
	env_node = get_env_node(env, key);
	if (env_node)
	{
		free_str_arrays(env_node->values);
		env_node->values = ft_split(get_values_pos(argv[(i)]), ':');
		if (!env_node->values)
			return (free(key), print_err(1, 1, "malloc in export"), 0);
	}
	else
		add_env_var(&env, key, (char *)get_values_pos(argv[(i)]));
	free(key);
	return (1);
}

/**
 * @brief Export
 */
void	ft_export(t_env *env, int argc, char **argv)
{
	int		i;

	if (argc == 1)
		return (print_export(env));
	i = 1;
	while (i < argc)
	{
		if (ft_strchr((const char *) argv[i], '='))
		{
			if (!export_variable(env, argv, i))
				return ;
		}
		else if (!ft_isalpha(argv[i][0]))
			return (g_signal = 1, (void) ft_printf("shelld0n[1]: export: `%s':\
not a valid identifier\n", argv[i]));
		else if (!get_env_node(env, argv[i]))
		{
			add_env_var(&env, argv[i], NULL);
			if (!get_env_node(env, argv[i])->key)
				return (print_err(1, 1, "malloc in export"));
		}
		i++;
	}
	g_signal = 0;
}
