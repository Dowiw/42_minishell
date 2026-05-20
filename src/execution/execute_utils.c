#include "minishell.h"

/**
 * @brief Unlink heredocs and return to ROM
 */
void	unlink_heredocs(t_cmd *cmds)
{
	t_cmd	*c;
	t_redir	*r;

	c = cmds;
	while (c)
	{
		r = c->redirs;
		while (r)
		{
			if (r->type == HEREDOC && r->file)
				unlink(r->file);
			r = r->next;
		}
		c = c->next;
	}
}
