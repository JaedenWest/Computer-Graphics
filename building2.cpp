//---------------------------------------
// Program: building2.cpp
// Purpose: Demonstrate x,y rotations of wall
//          model using keyboard callbacks.
// Author:  John Gauch
// Date:    Spring 2012
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global variables 
int xangle = 10;
int yangle = 15;
GLenum mode = GL_POLYGON;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-40.0, 40.0, -40.0, 40.0, -40.0, 40.0);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw a fixed thickness wall
//---------------------------------------
void wall(float x1, float y1, float x2, float y2)
{
   // Wall thickness and height
   float thickness = 0.25;
   float height = 7;
   float z1 = -height/2;
   float z2 =  height/2;

   // Calculate direction of wall
   float Dx = x2 - x1;
   float Dy = y2 - y1;
   float length = sqrt(Dx*Dx + Dy*Dy);
   Dx = thickness * Dx / length;
   Dy = thickness * Dy / length;

   // Calculate normal direction
   float Nx = Dy;
   float Ny = -Dx;

   // Extend wall endpoints
   x1 -= Dx;
   x2 += Dx;
   y1 -= Dy;
   y2 += Dy;

   // Draw wall polygons
   glColor3f(0.9, 0.1, 0.1);
   glBegin(mode);
   glVertex3f(x1+Nx, y1+Ny, z1);
   glVertex3f(x2+Nx, y2+Ny, z1);
   glVertex3f(x2+Nx, y2+Ny, z2);
   glVertex3f(x1+Nx, y1+Ny, z2);
   glEnd();
   glBegin(mode);
   glVertex3f(x1-Nx, y1-Ny, z1);
   glVertex3f(x2-Nx, y2-Ny, z1);
   glVertex3f(x2-Nx, y2-Ny, z2);
   glVertex3f(x1-Nx, y1-Ny, z2);
   glEnd();
   glBegin(mode);
   glVertex3f(x1-Nx, y1-Ny, z1);
   glVertex3f(x1+Nx, y1+Ny, z1);
   glVertex3f(x1+Nx, y1+Ny, z2);
   glVertex3f(x1-Nx, y1-Ny, z2);
   glEnd();
   glBegin(mode);
   glVertex3f(x2-Nx, y2-Ny, z1);
   glVertex3f(x2+Nx, y2+Ny, z1);
   glVertex3f(x2+Nx, y2+Ny, z2);
   glVertex3f(x2-Nx, y2-Ny, z2);
   glEnd();
   glColor3f(0.1, 0.1, 0.9);
   glBegin(mode);
   glVertex3f(x1-Nx, y1-Ny, z1);
   glVertex3f(x2-Nx, y2-Ny, z1);
   glVertex3f(x2+Nx, y2+Ny, z1);
   glVertex3f(x1+Nx, y1+Ny, z1);
   glEnd();
   glBegin(mode);
   glVertex3f(x1-Nx, y1-Ny, z2);
   glVertex3f(x2-Nx, y2-Ny, z2);
   glVertex3f(x2+Nx, y2+Ny, z2);
   glVertex3f(x1+Nx, y1+Ny, z2);
   glEnd();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'x')
      xangle -= 5;
   else if (key == 'y') 
      yangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'Y') 
      yangle += 5;

   // Change display mode
   if (key == 'p')
      mode = GL_POLYGON;
   else if (key == 'l')
      mode = GL_LINE_LOOP;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);

   // Draw exterior walls
   wall(-18,18,24,18);
   wall(24,18,24,-15);
   wall(24,-15,0,-15);
   wall(-6,-15,-18,-15);
   wall(-18,-15,-18,18);

   // Draw interior walls
   wall(-6,-15,-6,-9); 
   wall(-6,-6,-18,-6);
   wall(-18,6,-15,6);
   wall(-9,6,-6,6);
   wall(-6,-3,-6,15);
   wall(0,0,12,0);
   wall(12,0,12,18);
   wall(0,-15,0,-6);
   wall(3,-6,12,-6);
   wall(12,-6,12,-15);
   wall(15,-6,15,0);
   wall(15,0,24,0);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(800, 800);
   glutInitWindowPosition(200, 200);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Building");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   printf("Keyboard commands:\n");
   printf("   'x' - rotate x-axis -5 degrees\n");
   printf("   'X' - rotate x-axis +5 degrees\n");
   printf("   'y' - rotate y-axis -5 degrees\n");
   printf("   'Y' - rotate y-axis +5 degrees\n");
   printf("   'l' - display using lines\n");
   printf("   'p' - display using polygons\n");
   glutMainLoop();
   return 0;
}
