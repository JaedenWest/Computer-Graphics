//---------------------------------------
// Program: gasket3d.cpp
// Purpose: Recursive display of 3D Sierpenski
//          gasket using keyboard callbacks.
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

// Global variables
#define MAX_LEVEL 10
int level = 0;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-0.6, 0.6, -0.5, 0.7, -0.6, 0.6);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   // glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw 3D Sierpinski gasket
//---------------------------------------
void gasket(int n,
	    float ax, float ay, float az,
	    float bx, float by, float bz, float cx, float cy, float cz)
{
   // Handle recursive case
   if (n > 0)
   {
      float dx = (ax + bx) / 2;
      float dy = (ay + by) / 2;
      float dz = (az + bz) / 2;
      float ex = (bx + cx) / 2;
      float ey = (by + cy) / 2;
      float ez = (bz + cz) / 2;
      float fx = (cx + ax) / 2;
      float fy = (cy + ay) / 2;
      float fz = (cz + az) / 2;
      gasket(n - 1, ax, ay, az, dx, dy, dz, fx, fy, fz);
      gasket(n - 1, bx, by, bz, ex, ey, ez, dx, dy, dz);
      gasket(n - 1, cx, cy, cz, fx, fy, fz, ex, ey, ez);
   }

   // Draw single trangle
   else
   {
      glVertex3f(ax, ay, az);
      glVertex3f(bx, by, bz);
      glVertex3f(cx, cy, cz);
   }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   float ax = 0.0;
   float ay = 0.5;
   float az = -0.5;
   float bx = -0.43;
   float by = -0.25;
   float bz = -0.5;
   float cx = 0.43;
   float cy = -0.25;
   float cz = -0.5;
   float dx = 0.0;
   float dy = 0.0;
   float dz = 0.0;

   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT );
   glBegin(GL_TRIANGLES);

   glColor3f(0.5, 0.5, 0.5);
   gasket(level, ax, ay, az, bx, by, bz, cx, cy, cz);

   glColor3f(1.0, 0.0, 0.0);
   gasket(level, dx, dy, dz, ax, ay, az, bx, by, bz);

   glColor3f(0.0, 1.0, 0.0);
   gasket(level, dx, dy, dz, bx, by, bz, cx, cy, cz);

   glColor3f(0.0, 0.0, 1.0);
   gasket(level, dx, dy, dz, cx, cy, cz, ax, ay, az);

   glEnd();
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   if (key == '+')
      level = (level + 1) % MAX_LEVEL;
   else if (key == '-')
      level = (level + MAX_LEVEL - 1) % MAX_LEVEL;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   // glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
   glutCreateWindow("Gasket3D");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   printf("Keyboard commands:\n");
   printf("   '+' - increase recursion level by 1\n");
   printf("   '-' - decrease recursion level by 1\n");
   glutMainLoop();
   return 0;
}
