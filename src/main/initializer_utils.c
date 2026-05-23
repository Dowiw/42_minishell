/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 19:28:31 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 13:21:03 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Wrapper for variable modifier.
 *
 * @returns 1 for errors, 0 for success
 */
int	modify_variables(t_env **curr)
{
	int		temp;

	if (ft_strcmp((*curr)->key, "SHLVL") == 0)
	{
		temp = ft_atoi((*curr)->values[0]) + 1;
		free((*curr)->values[0]);
		(*curr)->values[0] = ft_itoa(temp);
		if (!(*curr)->values[0])
			return (1);
	}
	else if (ft_strcmp((*curr)->key, "PWD") == 0)
	{
		free((*curr)->values[0]);
		(*curr)->values[0] = getcwd(NULL, 0);
		if (!(*curr)->values[0])
			return (1);
	}
	else if (ft_strcmp((*curr)->key, "OLDPWD") == 0)
	{
		free((*curr)->values[0]);
		(*curr)->values[0] = NULL;
	}
	return (0);
}
