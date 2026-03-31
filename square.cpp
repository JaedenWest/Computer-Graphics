//---------------------------------------
// Program: square.cpp
// Purpose: Demonstrate basic OpenGL commands
//          to display two squares.
// Author:  John Gauch
// Date:    September 2008
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
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLineWidth(3);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_LINE_LOOP);
   glVertex2f(-0.5, -0.5);
   glVertex2f(-0.5, 0.5);
   glVertex2f(0.5, 0.5);
   glVertex2f(0.5, -0.5);
   glEnd();

   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(-0.25, -0.25);
   glVertex2f(-0.25, 0.25);
   glVertex2f(0.25, 0.25);
   glVertex2f(0.25, -0.25);
   glEnd();

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
   glutCreateWindow("Square");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
