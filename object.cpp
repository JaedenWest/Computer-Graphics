//---------------------------------------
// Program: object.cpp
// Purpose: Generate and display 3D model
//          of an object of rotation.
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

// Define global variables
#define SIZE 36
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
int angle = 0;

//---------------------------------------
// Define donut surface
//---------------------------------------
void define_donut()
{
   // Create X-Y profile
   for (int i = 0; i <= SIZE; i++)
   {
      float theta = i * 2 * M_PI / SIZE;
      Px[i][0] = 0.2 * cos(theta) + 0.5;
      Py[i][0] = 0.2 * sin(theta);
      Pz[i][0] = 0.0;
   }

   // Perform rotation around Y axis
   for (int j = 1; j <= SIZE; j++)
   {
      float theta = j * 2 * M_PI / SIZE;
      float cos_theta = cos(theta);
      float sin_theta = sin(theta);
      for (int i = 0; i <= SIZE; i++)
      {
	 Px[i][j] = Px[i][0] * cos_theta - Pz[i][0] * sin_theta;
	 Py[i][j] = Py[i][0];
	 Pz[i][j] = Px[i][0] * sin_theta + Pz[i][0] * cos_theta;
      }
   }
}

//---------------------------------------
// Define vase surface
//---------------------------------------
void define_vase()
{
   // Create X-Y profile
   for (int i = 0; i <= SIZE; i++)
   {
      float theta = i * 2 * M_PI / SIZE;
      Px[i][0] = 0.2 * sin(theta) + 0.5;
      Py[i][0] = (float) i / (float) SIZE - 0.5;
      Pz[i][0] = 0.0;
   }

   // Perform rotation around Y axis
   for (int j = 1; j <= SIZE; j++)
   {
      float theta = j * 2 * M_PI / SIZE;
      float cos_theta = cos(theta);
      float sin_theta = sin(theta);
      for (int i = 0; i <= SIZE; i++)
      {
	 Px[i][j] = Px[i][0] * cos_theta - Pz[i][0] * sin_theta;
	 Py[i][j] = Py[i][0];
	 Pz[i][j] = Px[i][0] * sin_theta + Pz[i][0] * cos_theta;
      }
   }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glEnable(GL_DEPTH_TEST);

   // Define surface
   // define_vase();
   define_donut();
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
   glRotatef(angle, 1.0, 0.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0);
   angle = (angle + 5) % 360;

   // Draw object surface
   glColor3f(1.0, 1.0, 1.0);
   for (int i = 0; i <= SIZE; i++)
   {
      glBegin(GL_LINE_STRIP);
      for (int j = 0; j <= SIZE; j++)
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
      glEnd();
   }
   for (int j = 0; j <= SIZE; j++)
   {
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= SIZE; i++)
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
      glEnd();
   }
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
   glutCreateWindow("Object");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
