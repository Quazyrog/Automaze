CC = g++
CC_FLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lSOIL -lglfw -lGL
OBJECTS = Main.o \
	  World.o \
	  Exception.o \
	  FindAndUnion.o \
	  GameWindow.o \
	  GLMatrix.o


.cpp.o:
	$(CC) $(CC_FLAGS) -c $< -o $@

all: $(OBJECTS)
	$(CC) $(LIBS) $(OBJECTS) -o Automaze.out

clean:
	@rm -rf *.o Automaze
