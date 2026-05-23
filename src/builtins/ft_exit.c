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
 * @brief Checking if the number is a valid integer
 */
static int	is_valid_numeric(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Exit.
 */
void	ft_exit(t_minishell *data_p, int argc, char **argv)
{
	long long	raw_code;

	ft_putstr_fd("exit\n", 1);
	if (argc > 1)
	{
		if (!is_valid_numeric(argv[1]))
			exit_err(0, 2, "exit: numeric argument required\n", data_p);
		if (argc > 2)
		{
			print_err(0, 1, "exit: too many arguments\n");
			return ;
		}
		raw_code = ft_atoi(argv[1]);
		g_signal = (int)((unsigned char)raw_code);
	}
	cleanup_shell(data_p);
	exit(g_signal);
}
