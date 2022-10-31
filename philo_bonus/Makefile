# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/09 15:07:02 by daejlee           #+#    #+#              #
#    Updated: 2022/09/09 15:07:02 by daejlee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME 		= philo
CFLAGS 		= -Wall -Wextra -Werror
CC 			= cc
SRC 		= philo_main_bonus.c philo_utils_1_bonus.c philo_utils_2_bonus.c philo_grab_n_eat_bonus.c
OBJ 		= $(SRC:.c=.o)
LIBFT 		= ./libft_garage/libft.a
LIBFT_DIR	= ./libft_garage
MLX_DIR		= ./mlx
MLX_FLAGS	= -L$(MLX_DIR) -lmlx -framework OpenGL -framework Appkit

ifdef WITH_BONUS
	OBJ_FILES = $(BONUS_OBJ)
	SRC_FILES = $(BONUS_SRC)
else
	OBJ_FILES = $(OBJ)
	SRC_FILES = $(SRC)
endif

all : $(NAME)

$(NAME) : $(OBJ_FILES)
	$(CC) -g -pthread $(CFLAGS) -o $(NAME) $(SRC)
#	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) -lft -L$(LIBFT_DIR)

$(OBJ_FILES) : $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

clean :
	rm -f $(OBJ_FILES) $(BONUS_OBJ)
#	make -C libft_garage/ clean

fclean :
	rm -f $(OBJ_FILES) $(BONUS_OBJ) $(NAME)
#	make -C libft_garage/ fclean

re :
	$(MAKE) fclean
	$(MAKE) all
