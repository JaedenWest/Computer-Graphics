//---------------------------------------
// Program: building.cpp
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
int zangle = 5;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw a zero thickness wall
//---------------------------------------
void wall(float x1, float y1, float x2, float y2)
{
   float z1 = 0;
   float z2 = 10;

   // Draw wall polygon
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();

   // Draw wall outline
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_LOOP);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'q')
      exit(0);
   else if (key == 'x')
      xangle -= 5;
   else if (key == 'y') 
      yangle -= 5;
   else if (key == 'z') 
      zangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'Y') 
      yangle += 5;
   else if (key == 'Z') 
      zangle += 5;
   
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
   glRotatef(zangle, 0.0, 0.0, 1.0);

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
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
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
   printf("   'z' - rotate z-axis -5 degrees\n");
   printf("   'Z' - rotate z-axis +5 degrees\n");
   glutMainLoop();
   return 0;
}
