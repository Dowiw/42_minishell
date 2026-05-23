/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw_mode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:44 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 00:05:01 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Wrap the disable for outer terminal.
 * Exits when errors occur
 */
void	disable_raw_mode(t_minishell *data)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &data->orig_settings))
		exit_err(1, 1, "disable_raw_mode tcsetattr error", data);
}

/**
 * @brief Enable raw attributes for current terminal.
 * Exits when errors occur
 */
void	enable_raw_mode(t_minishell *data)
{
	struct termios	raw;

	if (tcgetattr(STDIN_FILENO, &data->orig_settings) < 0)
		exit_err(1, 1, "enable_raw_mode tcsetattr error", data);
	raw = data->orig_settings;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
		exit_err(1, 1, "enable_raw_mode tcsetattr error", data);
}
