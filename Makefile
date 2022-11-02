# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: daejlee <daejlee@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/09 15:07:02 by daejlee           #+#    #+#              #
#    Updated: 2022/11/02 13:32:20 by daejlee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME 		= philo
CFLAGS 		= -Wall -Wextra -Werror
CC 			= cc
SRC 		= philo_main.c philo_utils_1.c philo_utils_2.c philo_grab_n_eat.c
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
	make -C ./philo_bonus/ clean

fclean :
	rm -f $(OBJ_FILES) $(BONUS_OBJ) $(NAME)
	make -C ./philo_bonus/ fclean

re :
	$(MAKE) fclean
	$(MAKE) all

bonus :
	cd ./philo_bonus; $(MAKE)
