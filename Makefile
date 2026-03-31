# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = libim/libim.a jpeg/libjpeg.a

# define makefile targets
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3
BIN =  Homework4 Homework3 Homework2 Homework1 balls balls2 balls3 balls4 basic bezier bitmap building building2 building3 callback clip clip2 city city2 cube cube2 cube3 cube4 cube5 curve disk draw_object gasket2d gasket3d gouraud keyboard lagrange line_split line_split2 line_test mandelbrot mandelbrot2 object object2 object3 object4 object5 office office2 office3 office4 pong rectangles render render2 render3 show_poly square square2 surface surface2 surface3 surface4 tetris tetris2 tetris3 texture texture2 texture3 texture4 texture5 tree triangle_split whack_a_mole 

all: $(LIB2) $(BIN)

clean: 
	/bin/rm -f */*.o $(LIB2) $(BIN)

jpeg/libjpeg.a:
	cd jpeg;make;cd ..

libim/libim.a:
	cd libim;make;cd ..

balls: balls.cpp shading.cpp
	$(CC) -o balls balls.cpp $(LIB)

balls2: balls2.cpp shading.cpp
	$(CC) -o balls2 balls2.cpp $(LIB)

balls3: balls3.cpp shading.cpp
	$(CC) -o balls3 balls3.cpp $(LIB)

balls4: balls4.cpp shading.cpp
	$(CC) -o balls4 balls4.cpp $(LIB)

basic: basic.cpp
	$(CC) -o basic basic.cpp $(LIB)

bezier: bezier.cpp
	$(CC) -o bezier bezier.cpp $(LIB)

bitmap: bitmap.cpp
	$(CC) -o bitmap bitmap.cpp $(LIB)

building: building.cpp
	$(CC) -o building building.cpp $(LIB)

building2: building2.cpp
	$(CC) -o building2 building2.cpp $(LIB)

building3: building3.cpp
	$(CC) -o building3 building3.cpp $(LIB)

callback: callback.cpp
	$(CC) -o callback callback.cpp $(LIB)

clip: clip.cpp
	$(CC) -o clip clip.cpp $(LIB)

clip2: clip2.cpp
	$(CC) -o clip2 clip2.cpp $(LIB)

city: city.cpp
	$(CC) -o city city.cpp $(LIB)

city2: city2.cpp
	$(CC) -o city2 city2.cpp $(LIB)

cube: cube.cpp
	$(CC) -o cube cube.cpp $(LIB)

cube2: cube2.cpp
	$(CC) -o cube2 cube2.cpp $(LIB)

cube3: cube3.cpp
	$(CC) -o cube3 cube3.cpp $(LIB)

cube4: cube4.cpp
	$(CC) -o cube4 cube4.cpp $(LIB)

cube5: cube5.cpp
	$(CC) -o cube5 cube5.cpp $(LIB)

curve: curve.cpp
	$(CC) -o curve curve.cpp $(LIB)

disk: disk.cpp
	$(CC) -o disk disk.cpp $(LIB)

draw_object: draw_object.cpp
	$(CC) -o draw_object draw_object.cpp $(LIB)

gasket2d: gasket2d.cpp
	$(CC) -o gasket2d gasket2d.cpp $(LIB)

gasket3d: gasket3d.cpp
	$(CC) -o gasket3d gasket3d.cpp $(LIB)

gouraud: gouraud.cpp
	$(CC) -o gouraud gouraud.cpp $(LIB)

keyboard: keyboard.cpp
	$(CC) -o keyboard keyboard.cpp $(LIB)

lagrange: lagrange.cpp
	$(CC) -o lagrange lagrange.cpp $(LIB)

line_split: line_split.cpp
	$(CC) -o line_split line_split.cpp $(LIB)

line_split2: line_split2.cpp
	$(CC) -o line_split2 line_split2.cpp $(LIB)

line_test: line_test.cpp timer.cpp
	$(CC) -o line_test line_test.cpp timer.cpp

mandelbrot: mandelbrot.cpp
	$(CC) -o mandelbrot mandelbrot.cpp $(LIB)

mandelbrot2: mandelbrot2.cpp
	$(CC) -o mandelbrot2 mandelbrot2.cpp $(LIB)

object: object.cpp
	$(CC) -o object object.cpp $(LIB)

object2: object2.cpp
	$(CC) -o object2 object2.cpp $(LIB)

object3: object3.cpp
	$(CC) -o object3 object3.cpp $(LIB)

object4: object4.cpp shading.cpp
	$(CC) -o object4 object4.cpp $(LIB)

object5: object5.cpp 
	$(CC) -o object5 object5.cpp $(LIB)

office: office.cpp
	$(CC) -o office office.cpp $(LIB)

office2: office2.cpp
	$(CC) -o office2 office2.cpp $(LIB)

office3: office3.cpp
	$(CC) -o office3 office3.cpp $(LIB)

office4: office4.cpp
	$(CC) -o office4 office4.cpp $(LIB)

pong: pong.cpp
	$(CC) -o pong pong.cpp $(LIB)

rectangles: rectangles.cpp
	$(CC) -o rectangles rectangles.cpp $(LIB)

render: render.cpp
	$(CC) -o render render.cpp $(LIB)

render2: render2.cpp
	$(CC) -o render2 render2.cpp $(LIB)

render3: render3.cpp
	$(CC) -o render3 render3.cpp $(LIB)

show_poly: show_poly.cpp
	$(CC) -o show_poly show_poly.cpp $(LIB)

square: square.cpp
	$(CC) -o square square.cpp $(LIB)

square2: square2.cpp
	$(CC) -o square2 square2.cpp $(LIB)

surface: surface.cpp
	$(CC) -o surface surface.cpp $(LIB)

surface2: surface2.cpp
	$(CC) -o surface2 surface2.cpp $(LIB)

surface3: surface3.cpp shading.cpp
	$(CC) -o surface3 surface3.cpp $(LIB)

surface4: surface4.cpp shading.cpp
	$(CC) -o surface4 surface4.cpp $(LIB)

tetris: tetris.cpp
	$(CC) -o tetris tetris.cpp $(LIB)

tetris2: tetris2.cpp
	$(CC) -o tetris2 tetris2.cpp $(LIB)

tetris3: tetris3.cpp
	$(CC) -o tetris3 tetris3.cpp $(LIB)

texture: texture.cpp
	$(CC) -o texture texture.cpp $(LIB)

texture2: texture2.cpp
	$(CC) -o texture2 texture2.cpp $(LIB)

texture3: texture3.cpp $(LIB2)
	$(CC) -o texture3 texture3.cpp $(LIB) $(LIB2)

texture4: texture4.cpp $(LIB2)
	$(CC) -o texture4 texture4.cpp $(LIB) $(LIB2)

texture5: texture5.cpp $(LIB2)
	$(CC) -o texture5 texture5.cpp $(LIB) $(LIB2)

tree: tree.cpp
	$(CC) -o tree tree.cpp $(LIB)

triangle_split: triangle_split.cpp
	$(CC) -o triangle_split triangle_split.cpp $(LIB)

whack_a_mole: whack_a_mole.cpp timer.cpp
	$(CC) -o whack_a_mole whack_a_mole.cpp timer.cpp $(LIB)

Homework1: Homework1.cpp 
	$(CC) -o Homework1 Homework1.cpp $(LIB)

Homework2: Homework2.cpp 
	$(CC) -o Homework2 Homework2.cpp $(LIB)

Homework3: Homework3.cpp 
	$(CC) -o Homework3 Homework3.cpp $(LIB)

Homework4: Homework4.cpp $(LIB2)
	$(CC) -o Homework4 Homework4.cpp $(LIB) $(LIB2)
