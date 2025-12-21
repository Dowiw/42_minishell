#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h" // contains unistd.h

/**
 *
 */
void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

/**
 *
 */
int main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;

	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		write(STDOUT_FILENO, "~$ ", 4);
		line = get_next_line(STDIN_FILENO);
		if (!line) // Ctrl + D?
			break;
		if (line[0] == '\n')
		{
			free(line);
			continue;
		}
		tokens = ft_split(line, ' ');
		if (tokens)
		{
			for (int i = 0; tokens[i]; i++)
				printf("Token: %s\n", tokens[i]);
		}
		printf("%s", line);
		free_tokens(tokens);
		free(line);
	}
	printf("exit\n");
	return (0);
}
