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
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief For printing error but letting the shell pass.
 * For err strings without b_perror its recommended to put a newlin on the end
 * For string with b_perror you must remove the colon at the end
 *
 * @param b_perror if print perror with the err string
 * @param err_no the error number
 * @param err the string to print
 */
void	print_err(int b_perror, int err_no, char *err)
{
	char	*err_print;

	ft_putstr_fd("shelld0n[", STDERR_FILENO);
	err_print = ft_itoa(err_no);
	if (!err_print)
	{
		ft_putstr_fd("shelld0n[1]: ", STDERR_FILENO);
		perror("malloc in exit: ");
		exit(1);
	}
	ft_putstr_fd(err_print, STDERR_FILENO);
	ft_putstr_fd("]: ", STDERR_FILENO);
	free(err_print);
	if (b_perror)
		perror(err);
	else
		ft_putstr_fd(err, STDERR_FILENO);
	g_signal = err_no;
}

/**
 * @brief Print out err in stdout.
 * Frees data. If printing out perror, make sure to have a colon at the end.
 *
 * @param b_perror manages perror is changed in context
 * @param err_no is the signal to exit
 * @param err is the string to print out for the error
 * @param data the minishell data
 */
void	exit_err(int b_perror, int err_no, char *err, t_minishell *data)
{
	if (err)
		print_err(b_perror, err_no, err);
	cleanup_shell(data);
	g_signal = err_no;
	exit(err_no);
}
