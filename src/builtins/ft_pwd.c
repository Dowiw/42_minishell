/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:41 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:25:47 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <limits.h>
#include <stdio.h>

/**
 * @brief PWD
 */
void	ft_pwd(void)
{
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		print_err(1, 1, "getcwd in pwd");
	else
		ft_printf("%s\n", path);
	g_signal = 0;
}
