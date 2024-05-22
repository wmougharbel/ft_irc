NAME = ircserv

CPP = c++

CPPFLAGS = -g -Wall -Wextra -Werror -g -std=c++98 -pedantic #-fsanitize=address

RM = rm -rf

SRC_DIR = sources/

SOURCES = $(addprefix $(SRC_DIR),   validateInput.cpp \
									Client.cpp \
									Server.cpp \
									Channel.cpp \
									Mode.cpp)

OBJECTS = $(addsuffix .o, $(basename $(SOURCES)))

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CPP) $(CPPFLAGS) $(OBJECTS) -o $(NAME)
	@echo "\n==========Compiled Successfully==========\n"
	@sleep 1
	@clear

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -I. -c $< -o $@

clean:
	@$(RM) $(OBJECTS)

fclean: clean
	@$(RM) $(NAME)
	@echo "\n==========Cleaned Successfully==========\n"
	@sleep 1
	@clear

re: fclean all