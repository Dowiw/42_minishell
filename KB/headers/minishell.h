// only do last
#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h" // contains unistd.h
#include "ft_printf.h"
#include "minishell.h"

extern volatile sig_atomic_t g_signal;

typedef struct s_env_vars
{
	char		*key;
	char		**values;
	struct s_env_vars	*next;
}	t_env_vars;

/* inbuilts.c */


void 	ft_echo(char **argv);
void	ft_cd(int argc, char **argv);
void 	ft_pwd();

/* env_init.c */

void	initialize_env(t_env_vars **env_copy, char **envp);

/* execute.c */

void	perform_tokens(t_list *tokens, t_env_vars *copy, char **envp);
void	perform(char *current, t_list *tokens, char **envp);

/* signals.c */

void	setup_signals(void);

/* parsing.c */

char	*listen_input(int fd);
void	initialize(int argc, char **argv, t_env_vars *env_copy, char **envp);

/* tokens.c */

t_list	*parse_tokens(char *input);

#endif
