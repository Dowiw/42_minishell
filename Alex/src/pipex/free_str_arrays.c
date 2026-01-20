/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_str_arrays.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 19:01:19 by anematol          #+#    #+#             */
/*   Updated: 2025/09/26 19:01:44 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_str_arrays(char **str_array1, char **str_array2)
{
	free_str_array(str_array1);
	free_str_array(str_array2);
}
