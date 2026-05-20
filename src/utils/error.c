/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:00 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 14:19:07 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/**
 * @brief Print out err in stdout.
 */
void	exit_err(char *err, t_minishell *data)
{
	ft_putstr_fd("shelld0n[1]: ", STDERR_FILENO);
	perror(err);
	cleanup_shell(data);
	exit(EXIT_FAILURE);
}
