//---------------------------------------
// Program: line_split2.cpp
// Purpose: Generate random line sequences
//          using recursive splitting.
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
#define SIZE 32
float X[SIZE + 1];
float Y[SIZE + 1];

//---------------------------------------
// Calculate random value between [-R..R]
//---------------------------------------
float myrand(float R)
{
   return (2 * R * rand()) / RAND_MAX - R;
}

//---------------------------------------
// Recursive function to split lines
//---------------------------------------
void split(int low, int high, float rough)
{
   // Check terminating condition
   if ((high - low) > 1)
   {
      // Calculate length of line segment
      float dx = X[high] - X[low];
      float dy = Y[high] - Y[low];
      float displacement = sqrt(dx * dx + dy * dy) * rough;

      // Generate midpoint with random displacement
      int mid = (high + low) / 2;
      X[mid] = (X[high] + X[low]) / 2 + myrand(displacement);
      Y[mid] = (Y[high] + Y[low]) / 2 + myrand(displacement);

      // Perform recursive calls
      split(low, mid, rough);
      split(mid, high, rough);
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
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   int i;
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw red line
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);
   X[0] = -0.5;
   Y[0] = -0.5;
   X[SIZE] = 0.5;
   Y[SIZE] = 0.5;
   split(0, SIZE, 0.3);
   for (i = 0; i <= SIZE; i++)
      glVertex2f(X[i], Y[i]);
   glEnd();

   // Draw green line
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   X[0] = 0.5;
   Y[0] = -0.5;
   X[SIZE] = -0.5;
   Y[SIZE] = 0.5;
   split(0, SIZE, 0.1);
   for (i = 0; i <= SIZE; i++)
      glVertex2f(X[i], Y[i]);
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
   glutCreateWindow("Line-Split");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
