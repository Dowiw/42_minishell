#include "minishell.h"

/**
 *
 */
void	perform_tokens(t_list *tokens)
{

}

/**
 *
 */
void	free_tokens(t_list *tokens)
{
	ft_lstclear(&tokens, free);
}

t_list	*parse_tokens(char *input)
{
	int		i;
	t_list	*tokens;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while(ft_isspace(input[i]))
			i++;
		if (!input[i]) // exit if input is end
			break;

		// handle quotes
		if (input[i] == '\'' || input[i] == '"')
		{
			char quote = input[i++];
			int start = i;
			while (input[i] && input[i] != quote)
				i++;
			int len = i - start;
			char *token = malloc(len + 1);
			ft_strncpy(token, &input[start], len);
			token[len] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			if (input[i] == quote)
				i++;
		}

		// handle redirection operators
		else if ((input[i] == '<' && input[i+1] == '<') || (input[i] == '>' && input[i+1] == '>'))
		{
			char *token = malloc(3);
			token[0] = input[i];
			token[1] = input[i+1];
			token[2] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i += 2;
		}

		// only one redirection
		else if (input[i] == '<' || input[i] == '>')
		{
			char *token = malloc(2);
			token[0] = input[i];
			token[1] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i++;
		}

		// handle normal cmds
		else
		{
			int start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' &&
				input[i] != '<' && input[i] != '>' && input[i] != '\'' && input[i] != '"')
				i++;
			int len = i - start;
			char *token = malloc(len + 1);
			ft_strncpy(token, &input[start], len);
			token[len] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
		}
	}
	return (tokens);
}

void	print_tokens(t_list *tokens)
{
	t_list *cursor;

	cursor = tokens;
	while (cursor != NULL)
	{
		printf("%s\n", (char *)cursor->content);
		cursor = cursor->next;
	}
}
