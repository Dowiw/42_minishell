#include "minishell.h"

/**
 * @brief
 */
void	initialize_env(t_env_vars **env_copy, char **envp)
{
	int			i;
	t_env_vars	*curr;
	t_env_vars	*prev;

	curr = NULL;
	prev = NULL;
	while (envp && *envp != NULL)
	{
		curr = malloc(sizeof(t_env_vars)); // add check
		i = 0;
		while ((*envp)[i] != '=')
			i++;
		curr->key = malloc(sizeof(char) * (i + 1)); // add
		ft_strncpy(curr->key, *envp, i);
		curr->key[i] = '\0';
		curr->values = ft_split(*envp + i + 1, ':'); // check
		curr->next = NULL;
		if (*env_copy == NULL)
			*env_copy = curr;
		else
			prev->next = curr;
		prev = curr;
		envp++;
	}
}

/**
 * @brief Initialize the environment & signal handlers.
 * Exits failure if too many arguments.
 */
void	initialize(int argc, char **argv, t_env_vars **env_copy, char **envp)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("Error: too many arguments\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (!envp && !*envp)
	{
		ft_putstr_fd("Error: env variables don't exist\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	setup_signals();
	initialize_env(env_copy, envp);
}

/**
 *
 */
char	*listen_input(int fd)
{
	char	*input;

	write(1, "~$ ", 3);
	input = get_next_line(fd);
	if (input == NULL)
	{
		get_next_line(-1);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (NULL);
	}
	return (input);
}
