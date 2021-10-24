NAME = webserver

CC = clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
# LEAKS = leaks
# SANITIZER = -fsanitize=address
# INC_DIR = ./includes
# SRC_DIR = ./srcs
# UTILS_DIR = ./utils
# INC = webserver.hpp

# -include
all:
$(NAME):
%.o: %.c
clean:
fclean:
re:
# debug:
.PHONY: all clean fclean re