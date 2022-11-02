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
NAME 		= philo_bonus
CFLAGS 		= -Wall -Wextra -Werror
CC 			= cc
SRC 		= philo_main_bonus.c philo_utils_1_bonus.c philo_utils_2_bonus.c philo_grab_n_eat_bonus.c
OBJ 		= $(SRC:.c=.o)

ifdef WITH_BONUS
	OBJ_FILES = $(BONUS_OBJ)
	SRC_FILES = $(BONUS_SRC)
else
	OBJ_FILES = $(OBJ)
	SRC_FILES = $(SRC)
endif

all : $(NAME)

$(NAME) : $(OBJ_FILES)
	$(CC) -pthread $(CFLAGS) -o $(NAME) $(SRC)

$(OBJ_FILES) : $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

clean :
	rm -f $(OBJ_FILES) $(BONUS_OBJ)

fclean :
	rm -f $(OBJ_FILES) $(BONUS_OBJ) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all
