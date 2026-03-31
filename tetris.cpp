//---------------------------------------
// Program: tetris.cpp
// Purpose: Draw seven tetris shapes.
// Author:  John Gauch
// Date:    Spring 2021
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
using namespace std;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

//---------------------------------------
// Draw one square
//---------------------------------------
void draw_square(float x, float y, float w, float R, float G, float B)
{
   glBegin(GL_POLYGON);
   glColor3f(R, G, B);
   glVertex2f(x-w/2, y-w/2);
   glVertex2f(x-w/2, y+w/2);
   glVertex2f(x+w/2, y+w/2);
   glVertex2f(x+w/2, y-w/2);
   glEnd();

   glLineWidth(3);
   glBegin(GL_LINE_LOOP);
   glColor3f(R/2, G/2, B/2);
   glVertex2f(x-w/2, y-w/2);
   glVertex2f(x-w/2, y+w/2);
   glVertex2f(x+w/2, y+w/2);
   glVertex2f(x+w/2, y-w/2);
   glEnd();
}

//---------------------------------------
// Draw I piece
//---------------------------------------
void draw_I(float x, float y)
{
   float R=0.65, G=0.85, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x+2,y,1, R,G,B);
}

//---------------------------------------
// Draw J piece
//---------------------------------------
void draw_J(float x, float y)
{
   float R=0, G=0, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x-1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw L piece
//---------------------------------------
void draw_L(float x, float y)
{
   float R=1, G=0.65, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw O piece
//---------------------------------------
void draw_O(float x, float y)
{
   float R=1, G=1, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw T piece
//---------------------------------------
void draw_T(float x, float y)
{
   float R=1, G=0, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
}

//---------------------------------------
// Draw S piece
//---------------------------------------
void draw_S(float x, float y)
{
   float R=0, G=1, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw Z piece
//---------------------------------------
void draw_Z(float x, float y)
{
   float R=1, G=0, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x-1,y+1,1, R,G,B);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   draw_I(-6,2);
   draw_J(-1,2);
   draw_L(3,2);
   draw_O(6,2);
   draw_S(-4,-2);
   draw_T(0,-2);
   draw_Z(4,-2);

   glFlush();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display2()
{
   glClear(GL_COLOR_BUFFER_BIT);

   int size = 16;
   draw_I(random()%size-size/2, random()%size-size/2);
   draw_J(random()%size-size/2, random()%size-size/2);
   draw_L(random()%size-size/2, random()%size-size/2);
   draw_O(random()%size-size/2, random()%size-size/2);
   draw_S(random()%size-size/2, random()%size-size/2);
   draw_T(random()%size-size/2, random()%size-size/2);
   draw_Z(random()%size-size/2, random()%size-size/2);

   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Tetris");
   glutDisplayFunc(display2);
   init();
   glutMainLoop();
   return 0;
}
