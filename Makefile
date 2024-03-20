# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/03/20 15:20:27 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

NAME_BONUS = pipex_bonus

SRCS = ./srcs/childs_cmd_exec.c \
		./srcs/cmd_checking.c \
		./srcs/cmd_exec.c \
		./srcs/dup_close_childs.c \
		./srcs/error_handling.c \
		./srcs/free_funcs.c \
		./srcs/list_funcs.c \
		./srcs/main.c \
		./srcs/mem_alloc.c \
		./srcs/str_manipulation.c \
		./srcs/utils.c

SRCS_BONUS = ./srcs/childs_cmd_exec_bonus.c \
		./srcs/cmd_checking.c \
		./srcs/cmd_exec_bonus.c \
		./srcs/dup_close_childs_bonus.c \
		./srcs/error_handling.c \
		./srcs/free_funcs.c \
		./srcs/heredoc_handling_bonus.c \
		./srcs/list_funcs.c \
		./srcs/main_bonus.c \
		./srcs/mem_alloc.c \
		./srcs/str_manipulation.c \
		./srcs/utils.c

OBJ = $(SRCS:.c=.o)

OBJ_BONUS = $(SRCS_BONUS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror -g3

PATH_LIBFT = ./42-MEGALIBFT

NEW_PATH_LIBFT = .

LIBFT = ./42-MEGALIBFT/megalibft.a

all : 
	git submodule update --init
	make $(NAME)

$(NAME) : $(OBJ)
	make -C $(PATH_LIBFT)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -o $(NAME) -g3

%.o:%.c
	$(CC) $(FLAGS) -I $(PATH_LIBFT) -c $< -o $@

bonus : $(NAME_BONUS)

$(NAME_BONUS) : 
	git submodule update --init
	$(OBJ_BONUS)
	make -C $(PATH_LIBFT)
	$(CC) $(FLAGS) $(OBJ_BONUS) $(LIBFT) -o $(NAME_BONUS) -g3

clean :
	make -sC $(PATH_LIBFT) clean
	rm -f ./srcs/*o

fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

bonus : all

.PHONY : all bonus clean fclean re