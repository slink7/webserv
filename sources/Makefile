# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scambier <scambier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/30 15:23:35 by ellehmim          #+#    #+#              #
#    Updated: 2025/02/13 16:10:13 by scambier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++

CFlag = -Wall -Wextra -Werror -std=c++98 -g3 -I../headers/

SRC = main.cpp ConfigFile.cpp ConfigGroup.cpp Config.cpp Location.cpp Method.cpp FT.cpp
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