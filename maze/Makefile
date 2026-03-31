# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

maze: maze.cpp 
	$(CC) -o maze maze.cpp $(LIB) $(LIB2)

