# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/01 13:35:26 by anematol          #+#    #+#              #
#    Updated: 2025/09/26 19:06:01 by anematol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES = pipex.c\
		 ft_split.c\
		 ft_str_helpers.c\
		 pipex_helpers.c\
		 free_str_arrays.c

HFILES = pipex.h

CFLAGS = -Wall -Werror -Wextra

NAME = pipex

all: $(NAME)

$(NAME): $(CFILES) $(HFILES)
		cc $(CFLAGS) $(CFILES) -o $(NAME)

clean:
		rm -f $(NAME)

fclean:
		rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
