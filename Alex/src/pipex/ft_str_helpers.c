/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:37:17 by anematol          #+#    #+#             */
/*   Updated: 2025/09/18 13:02:45 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	int				is_equal;
	int				result;
	unsigned int	i;

	i = 0;
	is_equal = 1;
	result = 0;
	while (s1[i] != '\0' && i < n && is_equal == 1)
	{
		if (s1[i] != s2[i])
		{
			is_equal = 0;
			result = s1[i] - s2[i];
		}
		i++;
	}
	if (s1[i] != s2[i] && i < n && is_equal == 1)
	{
		is_equal = 0;
		result = s1[i] - s2[i];
	}
	return (result);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i_dest;
	int	i_src;

	i_dest = 0;
	i_src = 0;
	while (dest[i_dest] != '\0')
		i_dest++;
	while (src[i_src] != '\0')
	{
		dest[i_dest] = src[i_src];
		i_src++;
		i_dest++;
	}
	dest[i_dest] = '\0';
	return (dest);
}

char	*ft_strmerge_with_slash(char *str1, char *str2)
{
	int		total_length;
	char	*return_str;

	total_length = ft_strlen(str1) + ft_strlen(str2);
	return_str = ft_calloc(total_length + 2, sizeof(char));
	if (!return_str)
		return (NULL);
	return_str = ft_strcat(return_str, str1);
	return_str = ft_strcat(return_str, "/");
	return_str = ft_strcat(return_str, str2);
	return (return_str);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*ptr_s;
	unsigned char	uns_c;

	ptr_s = (unsigned char *)s;
	uns_c = (unsigned char)c;
	while (*ptr_s != '\0')
	{
		if (*ptr_s == uns_c)
			return ((char *)ptr_s);
		ptr_s++;
	}
	if (uns_c == 0)
		return ((char *)ptr_s);
	return (NULL);
}
