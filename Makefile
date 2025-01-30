#=== Project Config ===

NAME = webserv

COMPILER = c++

SRC_DIR = sources
SRC = \
	Method.cpp\
	main.cpp\
	FT.cpp\
	Message.cpp\
	Response.cpp\
	Request.cpp\
	CGI.cpp\
	Error.cpp\
	Proxy.cpp

CFLAGS = -Wall -Werror -Wextra -g3 -std=c++98
LFLAGS =

INCLUDES =\
	-I./\
	-Iheaders/\
	-Itemplates

OBJ_DIR = obj

#=== Automatic Vars ===

OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRC))))

#=== Targets ===

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(COMPILER) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(NAME) : $(OBJ_DIR) $(LIBS) $(OBJ)
	$(COMPILER) -o $(NAME) $(OBJ) $(LFLAGS)

clean :
	rm -rf $(OBJ_DIR) || true

fclean : clean
	rm -f $(NAME) $(NAME_BONUS) || true

re : fclean all

.PHONY : re fclean clean all