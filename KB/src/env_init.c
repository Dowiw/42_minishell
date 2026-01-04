#include <limits.h>
#include "minishell.h"

static t_env_vars	*first_pass(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0); // possible to pass PATH_MAX in size
	
}

/**
 *
 */
void	initialize_env(t_env_vars **env_copy, char **envp)
{
	if (*envp == NULL)
		(*env_copy) = first_shlvl();

}
