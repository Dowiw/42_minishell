/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:35:43 by anematol          #+#    #+#             */
/*   Updated: 2025/09/26 19:40:15 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>      // for perror, printf
# include <stdlib.h>     // for exit, EXIT_SUCCESS, EXIT_FAILURE
# include <unistd.h>     // for fork, execve, pipe, dup2, close, access, etc.
# include <sys/wait.h>   // for wait, waitpid
# include <errno.h>      // for errno variable
# include <string.h>     // for strerror
# include <fcntl.h>      // for open, close

// ft_split.c
void	*ft_calloc(size_t nmemb, size_t size);
void	free_str_array(char **str_array);
char	**ft_split(char const *s, char c);
char	*ft_split_dup(char const *s, char c);

// ft_str_helpers.c
int		ft_strncmp(char *s1, char *s2, unsigned int n);
int		ft_strlen(char *str);
char	*ft_strcat(char *dest, char *src);
char	*ft_strmerge_with_slash(char *str1, char *str2);
char	*ft_strchr(const char *s, int c);

// pipex_helpers.c
void	acc_error(char *full_cmd, int cmd_exists);
char	*get_first_word(char *str, char delimiter);
char	*find_cmd_path(char *full_cmd, char **cmd_args, char **paths);
int		check_local_access(char **cmd_p, char *full_cmd, char *path_variable);
int		set_cmd_path(char **envp, char *full_cmd, char **cmdpath);

//free_str_arrays.c
void	free_str_arrays(char **str_array1, char **str_array2);

#endif
