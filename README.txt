Install instructions:

0) Install WSL on your Windows machine
- follow instructions to install Ubuntu Linux
- type the following command to install OpenGL and the compiler
    sudo apt-get -y install mesa-common-dev freeglut3-dev g++ make
- enter the administrator password
- wait a few minutes for everything to download and install

1) Install OpenGL on your Linux machine
- open a terminal window
- type the following command to install OpenGL and the compiler
    sudo apt-get -y install mesa-common-dev freeglut3-dev g++ make
- enter the administrator password
- wait a few minutes for everything to download and install

2) Install 4813 sample code
- download and compile sample code using the following
    wget http://www.csce.uark.edu/~jgauch/4813/src/src.tar
    tar xvf src.tar
    cd src
    make clean
    make
- run any of the programs using ./program-name

3) Compile your own program on MacOS or Linux
- create your OpenGL program.cpp with favorite editor
- copy paste following into "Makefile"
- make sure you have a tab before $(CC) and NOT spaces
- run "make" to compile your graphics code
- type "./project" in command window to run your program

#-------------------------------------------------
# sample Makefile for OpenGL
#-------------------------------------------------

# define OpenGL libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif

# define compiler options
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3

project: project.cpp
	$(CC) -o project project.cpp $(LIB)

