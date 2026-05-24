/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:14 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:16:39 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/* INCLUDES                                                                   */
/* ========================================================================== */

# include <signal.h>
# include <termios.h>
# include <sys/types.h>
# include "libft.h"

/* ========================================================================== */
/* GLOBALS                                                                    */
/* ========================================================================== */

extern volatile sig_atomic_t	g_signal;

/* ========================================================================== */
/* ENUMERATIONS                                                               */
/* ========================================================================== */

/**
 * @brief Enumerations for the lexicographical part of the program
 */
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}	t_token_type;

/* ========================================================================== */
/* DATA STRUCTURES                                                            */
/* ========================================================================== */

/**
 * @brief A linked list for redirections inside the commands.
 *
 * @param type the type of redirection
 * @param file the filename
 * @param expand_heredoc a boolean to determine if the redirections should be
 * expanded
 * @param next the next node
 */
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				expand_heredoc;
	struct s_redir	*next;
}	t_redir;

/**
 * @brief A linked list for every command executed.
 *
 * @param args the argv of the command into execve
 * @param redirs the list of redirections related to the command
 * @param heredoc a boolean to determine if there is a heredoc in parsing
 * @param next the next node
 */
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

/**
 * @brief A linked list for the environment variables.
 *
 * @param key the key value of an env variable (any string before the equal sign)
 * @param values the values string after
 *  the equal sign split by the ':' delimiter, allocated
 * @param next the next node
 */
typedef struct s_env
{
	char			*key;
	char			**values;
	struct s_env	*next;
}	t_env;

/**
 * @brief A linked list for the tokens before converting them into commands.
 * Used for checking bad command tokens, syntax errors, etc.
 *
 * @param value the string of the token
 * @param type the enum type of the token
 * @param next the next node
 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/**
 * @brief A structure for storing input strings.
 *
 * @param history_count the number of strings in history.
 * @param history_index the current position of the user in history.
 * @param buffer the input string saved when browsing through history.
 * @param history a string array of inputs parsed by the shell process.
 */
typedef struct s_history
{
	unsigned long	history_count;
	unsigned long	history_index;
	char			*buffer;
	char			**history;
}	t_history;

/**
 * @brief shelld0n data
 *
 * @param input the string inputted from the prompt
 * @param history where history is stored
 * @param tokens where the input is stored after preprocessing
 * @param cmds command structure stored for each command passed
 * @param processed_env an env structure that is dynamic
 * @param pid pid of the current program, set in execution.c
 * @param heredoc_count the number of heredoc files in the current process
 * @param orig_settings termios structure of the outer terminal
 */
typedef struct s_minishell
{
	char			*input;
	t_history		history;
	t_token			*tokens;
	t_cmd			*cmds;
	t_env			*processed_env;
	pid_t			pid;
	int				heredoc_count;
	struct termios	orig_settings;
}	t_minishell;

/* ========================================================================== */
/* MAIN                                                                       */
/* ========================================================================== */
// main.c / initializer.c

int		initialize(int argc, char **argv, char **envp, t_minishell *data);

// prompt.c / raw_mode.c

void	write_prompt(void);
void	end_of_prompt(t_minishell *data);
void	init_prompt(t_minishell *data);
void	enable_raw_mode(t_minishell *data);
void	disable_raw_mode(t_minishell *data);

/* ========================================================================== */
/* INPUT                                                                      */
/* ========================================================================== */
// input.c / input_utils.c

char	*listen_input(int fd, t_minishell *data);
int		check_input(char *input);

// history.c

void	reset_history(t_history *history);
void	append_to_history(char **input, t_history *history);

// arrows.c / write_and_del.c

void	arrow_keys(t_history *history, char **input, long *cursor, long *len);
char	*delete_char(char *str, long pos);
char	*insert_char(char *str, char c, long pos);
void	backspace(char **input, long *cursor, long *input_len);
void	printable(char **input, char *c, long *cursor, long *input_len);

// signals.c

void	setup_signals(void);

/* ========================================================================== */
/* LEXER                                                                      */
/* ========================================================================== */
// lexer.c / tokens.c

t_token	*lexer(char *input);
t_list	*parse_tokens(char *input);
t_token	*new_token(char *value, t_token_type type);
void	add_token_back(t_token **list, t_token *new_node);
int		get_token_len(char *str);

int		handle_pipe_token(t_token **list);

/* ========================================================================== */
/* EXECUTION                                                                  */
/* ========================================================================== */
// execute.c / run_child.c

void	execute(t_cmd *cmds, t_minishell *data);
t_cmd	*tokens_to_cmds(t_token *tokens, t_minishell *data);
int		run_parent_inbuilt(t_cmd *cmd, t_minishell *data);
void	run_child(t_cmd *cmd, t_minishell *data, int prev_fd, int fd[2]);

// commands.c / commands_utils.c

t_cmd	*init_cmd(t_token *curr_start, t_minishell *data);
void	add_cmd_back(t_cmd **list, t_cmd *new_cmd);
int		count_args(t_token *curr, t_minishell *data);
t_redir	*new_redir_node(t_token_type type, char *file, int exp);
void	add_redir_back(t_redir **head, t_redir *new_node);

// heredoc.c

int		prep_all_heredocs(t_cmd *cmds, t_minishell *data);
void	unlink_heredocs(t_cmd *cmds);
int		has_quotes(char *str);
void	handle_heredoc_backspace(char *buffer, int *i);
int		manage_heredoc_chars(char *ch, char *buffer, int *i);
char	*expand_heredoc_body(char *str, t_env *env);

// expand.c / metacharacters.c

char	*expansion(char *str, t_env *env);
void	handle_expansion(char *str, int *i, char **res, t_env *env);
void	update_quote_state(char c, int *quote_state);
char	*strip_quotes_only(char *str);
void	append_char(char **res, char c);

// expand_utils.c

int	fill_word(t_token **tok, t_cmd **curr, int *i, t_minishell *data);


// path_resolver.c / check_syntax.c

char	*get_cmd_path(char *cmd, t_minishell *env_p);
int		check_syntax(t_token *tokens);

// redirs.c (handling file streams)

void	handle_pipes(t_cmd *cmd, int *prev_fd, int fd[2]);

/* ========================================================================== */
/* ENVIRONMENT                                                                */
/* ========================================================================== */
// environment.c

char	**convert_env_to_array(t_env *env_list);
t_env	*get_env_node(t_env *list, char *target_key);
char	*stitch_env_values(char **values);
int		modify_variables(t_env **curr);
int		add_env_var(t_env **copy, char *key, char *value);

/* ========================================================================== */
/* BUILTINS                                                                   */
/* ========================================================================== */
// inbuilts.c

void	ft_echo(int argc, char **argv);
void	ft_cd(t_env *env, int argc, char **argv);
void	ft_pwd(void);
void	ft_export(t_env *env, int argc, char **argv);
void	ft_unset(t_env **env, int argc, char **argv);
void	ft_env(t_env *env, int argc);
void	ft_exit(t_minishell *data_p, int argc, char **argv);

/* ========================================================================== */
/* UTILS                                                                      */
/* ========================================================================== */
// free_shell.c / cleanup.c

void	cleanup_loop(t_minishell *data);
void	cleanup_shell(t_minishell *data);
void	free_tokens(t_token *tokens);
void	free_env(t_env *env_list);
void	free_cmds(t_cmd *cmds);
void	free_redirs(t_redir *head);
void	free_str_arrays(char **str);

// error.c

void	exit_err(int b_perror, int err_no, char *err, t_minishell *data);
void	print_err(int b_perror, int err_no, char *err);

#endif
