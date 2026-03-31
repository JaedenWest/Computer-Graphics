# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif

# define makefile targets
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3

painter: *.cpp
	$(CC) -o painter *.cpp $(LIB)

clean:
	/bin/rm -f painter
