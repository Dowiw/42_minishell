/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/19 14:48:25 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <stdio.h>

/**
 * @brief changes PWD env variable
 */
static void	change_pwd(t_env *env)
{
	t_env		*old_pwd_node;
	const char	*export_argv[2];
	char		path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
	{
		print_err(1, 1, "unable to change pwd: getcwd");
		return ;
	}
	export_argv[0] = "export";
	export_argv[1] = "PWD";
	old_pwd_node = get_env_node(env, "PWD");
	if (!old_pwd_node)
	{
		ft_export(env, 2, (char **) export_argv);
		old_pwd_node = get_env_node(env, "PWD");
	}
	if (old_pwd_node && old_pwd_node->values)
		free_str_arrays(old_pwd_node->values);
	if (old_pwd_node)
	{
		old_pwd_node->values = ft_split(path, ':');
		if (!old_pwd_node->values)
			print_err(1, 1, "unable to malloc in change pwd: malloc");
	}
}

/**
 * @brief Changes OLDPWD env variable
 */
static void	change_oldpwd(t_env *env, char *old_path)
{
	t_env		*old_pwd_node;
	const char	*export_argv[2];

	export_argv[0] = "export";
	export_argv[1] = "OLDPWD";
	old_pwd_node = get_env_node(env, "OLDPWD");
	if (!old_pwd_node)
	{
		ft_export(env, 2, (char **) export_argv);
		old_pwd_node = get_env_node(env, "OLDPWD");
	}
	if (old_pwd_node && old_pwd_node->values)
		free_str_arrays(old_pwd_node->values);
	if (old_pwd_node)
	{
		old_pwd_node->values = ft_split(old_path, ':');
		if (!old_pwd_node->values)
			print_err(1, 1, "unable to malloc in change old pwd: malloc");
	}
}

/**
 * @brief Change directory without args
 */
static void	ft_cd_home(t_env *env, char *old_path)
{
	t_env	*home_node;
	char	*home_path;

	home_node = get_env_node(env, "HOME");
	if (!home_node)
	{
		print_err(0, 1, "cd: HOME not set\n");
		return ;
	}
	home_path = home_node->values[0];
	if (!home_path)
		return ;
	if (chdir(home_path) != 0)
	{
		print_err(1, 1, "unable to cd /home: chdir");
		return ;
	}
	change_oldpwd(env, old_path);
	change_pwd(env);
	g_signal = 0;
}

/**
 * @brief To check the home cd
 */
static int	check_home(t_env *env, int argc, char *old_path)
{
	if (argc == 1)
		return (ft_cd_home(env, old_path), 1);
	else if (argc > 2)
		return (print_err(0, 1, "cd: too many arguments\n"), 1);
	return (0);
}

/**
 * @brief Change directory
 */
void	ft_cd(t_env *env, int argc, char **argv)
{
	char	old_path[PATH_MAX];
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL || getcwd(old_path, PATH_MAX) == NULL)
		return (print_err(1, 1, "in cd: getcwd"));
	if (check_home(env, argc, old_path))
		return ;
	if (argv[1][0] == '/')
	{
		if (chdir(argv[1]) != 0)
			return (print_err(1, 1, "chdir in cd"));
		change_oldpwd(env, old_path);
		change_pwd(env);
		return ;
	}
	if (ft_strlen(path) + ft_strlen(argv[1]) + 1 > PATH_MAX)
		return (print_err(0, 1, "cd: ENAMETOOLONG\n"));
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, argv[1], PATH_MAX);
	if (chdir(path) != 0)
		return (print_err(1, 1, "cd: chdir"));
	else
		g_signal = 0;
	change_oldpwd(env, old_path);
	change_pwd(env);
}
