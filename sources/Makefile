# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/30 15:23:35 by ellehmim          #+#    #+#              #
#    Updated: 2025/02/02 10:01:02 by ellehmim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++

CFlag = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp ConfigFile.cpp ConfigGroup.cpp Config.cpp Location.cpp
OBJ = $(SRC:.cpp=.o)

NAME = pars

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFlag) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 