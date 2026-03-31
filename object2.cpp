//---------------------------------------
// Program: object2.cpp
// Purpose: Generate 3D model of an object
//          of rotation with surface normals.
// Author:  John Gauch
// Date:    September 2008, March 2010
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
#define SIZE 32
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];
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
// Define surface normals
//---------------------------------------
void define_normals()
{
   // Define surface normals
   for (int i = 0; i < SIZE; i++)
   for (int j = 0; j < SIZE; j++)
   {
      // Get two tangent vectors
      float Ix = Px[i+1][j] - Px[i][j];
      float Iy = Py[i+1][j] - Py[i][j];
      float Iz = Pz[i+1][j] - Pz[i][j];
      float Jx = Px[i][j+1] - Px[i][j];
      float Jy = Py[i][j+1] - Py[i][j];
      float Jz = Pz[i][j+1] - Pz[i][j];
      
      // Do cross product
      Nx[i][j] = Iy * Jz - Iz * Jy;
      Ny[i][j] = Iz * Jx - Ix * Jz;
      Nz[i][j] = Ix * Jy - Iy * Jx;
      float length = sqrt( 
         Nx[i][j] * Nx[i][j] + 
         Ny[i][j] * Ny[i][j] + 
         Nz[i][j] * Nz[i][j]);
      if (length > 0)
      {
         Nx[i][j] /= length;
         Ny[i][j] /= length;
         Nz[i][j] /= length;
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

   // Define surface and normals
   // define_vase();
   define_donut();
   define_normals();
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
   glColor3f(1.0, 1.0, 0.0);
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

   // Draw the normals
   float length = 0.05;
   glColor3f(0.0, 1.0, 1.0);
   glBegin(GL_LINES);
   for (int i = 0; i < SIZE; i++)
   for (int j = 0; j < SIZE; j++)
   {
      glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
      glVertex3f(Px[i][j]+length*Nx[i][j], 
         Py[i][j]+length*Ny[i][j], 
         Pz[i][j]+length*Nz[i][j]);
   }
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
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Object");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
