/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:41:37 by anematol          #+#    #+#             */
/*   Updated: 2026/05/20 11:38:51 by anematol         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

/**
 * @brief Env
 */
void	ft_env(t_env *env, int argc)
{
	int	i;

	if (argc > 1)
		return ;
	while (env)
	{
		if (*(env->values))
		{
			ft_printf("%s=", env->key);
			i = 0;
			while ((env->values)[i])
			{
				if (i > 0)
					ft_printf(":");
				ft_printf("%s", (env->values)[i]);
				i++;
			}
			ft_printf("\n");
		}
		env = env->next;
	}
}
