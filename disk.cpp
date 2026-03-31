//---------------------------------------
// Program: disk.cpp
// Purpose: Demonstrate color interpolation 
//          when displaying a disk.
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
int step = 0;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glOrtho(0, 1, 0, 1, 0, 1);
}

//---------------------------------------
// Function to get RGB color based on angle
//---------------------------------------
void get_color(int angle, float *R, float *G, float *B)
{
   // Interpolate between R and G
   float r=0, g=0, b=0;
   if ((angle >= 0) && (angle < 120))
   {
      float fraction = angle / 120.0;
      r = 1.0 - fraction;
      g = fraction;
      b = 0.0;
   }

   // Interpolate between G and B
   else if ((angle >= 120) && (angle < 240))
   {
      float fraction = (angle - 120) / 120.0;
      r = 0.0;
      g = 1.0 - fraction;
      b = fraction;
   }

   // Interpolate between B and R
   else if ((angle >= 240) && (angle < 360))
   {
      float fraction = (angle - 240) / 120.0;
      r = fraction;
      g = 0.0;
      b = 1.0 - fraction;
   }

   // Normalize length
   float length = sqrt(r * r + g * g + b * b);
   if (length > 0) r /= length;
   if (length > 0) g /= length;
   if (length > 0) b /= length;
   *R = r;
   *G = g;
   *B = b;
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // glColor3f(1.0, 0.0, 0.0);
    // glBegin(GL_POLYGON);
    // glVertex2f(0, 0);
    // glVertex2f(0, 1);
    // glVertex2f(1, 1);
    // glVertex2f(1, 0);
    // glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();

    glFlush();
}

void display2()
{
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_POLYGON);

   // Generate vertices on disk
   int angle = 0;
   if (++step > 60)
      step = 1;
   for (angle = 0; angle < 360; angle += step)
   {
      // Specify color
      float R, G, B;
      get_color(angle, &R, &G, &B);
      glColor3f(R, G, B);

      // Specify vertex
      float radians = M_PI * angle / 180.0;
      float X = 0.5 * cos(radians);
      float Y = 0.5 * sin(radians);
      glVertex2f(X, Y);
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
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Disk");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
