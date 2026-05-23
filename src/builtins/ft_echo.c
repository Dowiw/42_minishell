/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:30 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:23:15 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	while (str[i] != '\0' && str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	else
		return (0);
}

/**
 * @brief Echo
 */
void	ft_echo(int argc, char **argv)
{
	int	no_last_line;
	int	i;

	i = 1;
	no_last_line = 0;
	while (argv[i] && check_n_flag(argv[i]))
	{
		no_last_line = 1;
		i++;
	}
	while (i < argc)
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!no_last_line)
		ft_printf("\n");
	g_signal = 0;
}
