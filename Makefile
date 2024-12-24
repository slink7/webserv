# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ellehmim <ellehmim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/23 17:53:58 by ellehmim          #+#    #+#              #
#    Updated: 2024/12/24 22:06:28 by ellehmim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++

CFlag = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp server.cpp Location.cpp
OBJ = $(SRC:.cpp=.o)

NAME = webserv

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