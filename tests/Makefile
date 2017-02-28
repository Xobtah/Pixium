NAME	= WindowTest

SRC	= main.cpp \
	../src/Window.cpp \
	../Emitium/src/EventEmitter.cpp \
	../Poolium/src/Thread.cpp \

OBJ	= $(SRC:.cpp=.o)

CXXFLAGS = -Wall -std=c++11 -pthread

all: $(NAME)

$(NAME): $(OBJ)
	g++ -pthread -lSDL2 $^ -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
