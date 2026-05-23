#include "minishell.h"
#include "libft.h"

/**
 * @brief Handle the pipe token
 */
int	handle_pipe_token(t_token **list)
{
	t_token	*new;

	new = new_token(ft_strdup("|"), PIPE);
	if (!new)
		return (-1);
	add_token_back(list, new);
	return (1);
}

/**
 * @brief Updates the quote state integer based on the current character.
 * 0 = No quotes, 1 = Single quotes, 2 = Double quotes.
 */
void	update_quote_state(char c, int *quote_state)
{
	if (c == '\'' && *quote_state == 0)
		*quote_state = 1;
	else if (c == '\'' && *quote_state == 1)
		*quote_state = 0;
	else if (c == '\"' && *quote_state == 0)
		*quote_state = 2;
	else if (c == '\"' && *quote_state == 2)
		*quote_state = 0;
}
