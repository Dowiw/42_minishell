#include "minishell.h"

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
	setup_signals();
	initialize_env(env_copy, envp);
}


char	*parse_input(int fd)
{
	char	*input;

	write(1, "~$ ", 3);
	input = get_next_line(fd);
	if (input == NULL) // CTRL + D
	{
		get_next_line(-1);
		ft_printf("exit\n");
		return (NULL);
	}
	return (input);
}
