/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:42:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:24:45 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Exit
 */
void	ft_exit(t_minishell *data_p, int argc, char **argv)
{
	size_t	i;
	int		exit_code;

	i = 0;
	if (argc > 2)
		return ((void) ft_printf("exit: too many arguments\n"));
	if (argc == 1)
		exit_code = 0;
	else
	{
		while (argv[1][i])
		{
			if (!ft_isdigit(argv[1][i]))
				return (exit_err(0, 2, "numeric argument required\n", data_p));
			i++;
		}
		exit_code = ft_atoi(argv[1]);
	}
	cleanup_shell(data_p);
	exit(exit_code);
}
