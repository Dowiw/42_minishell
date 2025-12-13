#include <stdio.h>

// add libft
// only between echo and NULL
void	ft_echo(char **argv);

char	*read_cmd(void)
{
	char	*line;

	line = NULL;
}

/**
 * - REPL
 */
int main(void)
{
	char	*line;
	char	**args;

	while (line = read_cmd()) // test conditional
	{
		printf("%s\n", line);
	}
}
