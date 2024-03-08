# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/03/08 16:48:42 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS = ./pipex.c

OBJ = $(SRCS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror -g3

PATH_LIBFT = ./42-MEGALIBFT

NEW_PATH_LIBFT = .

LIBFT = ./42-MEGALIBFT/megalibft.a

all : $(NAME)

$(NAME) : $(OBJ)
	make -C $(PATH_LIBFT)
	# cp ./megalibft/megalibft.a ./megalibft.a
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -o $(NAME) -g3

%.o:%.c
	$(CC) $(FLAGS) -I $(PATH_LIBFT) -c $< -o $@

clean :
	make -sC $(PATH_LIBFT) clean
	rm -f *o

fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re