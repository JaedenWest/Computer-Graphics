//---------------------------------------
// Program: lagrange.cpp
// Purpose: Demonstrate curve fitting using
//          Lagrange polynomials.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global constants
#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 800
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 800

// Global variables 
#define MAX_COUNT 10
float Px[MAX_COUNT], Py[MAX_COUNT];
int count = -1;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse down
   if (state == GLUT_DOWN)
   {
      if (count < MAX_COUNT) count++; 
      Px[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      Py[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      glutPostRedisplay();
   }
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   Px[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   Py[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw control points
   glColor3f(1.0, 1.0, 1.0);
   glPointSize(3);
   glBegin(GL_POINTS);
   for (int i=0; i<=count; i++)
      glVertex2f(Px[i], Py[i]);
   glEnd();

   // Connect control points
   glLineWidth(2);
   glBegin(GL_LINE_STRIP);
   for (int i=0; i<=count; i++)
      glVertex2f(Px[i], Py[i]);
   glEnd();

   // Draw lagrange polynomial curve
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
   for (float t_val = 0.0; t_val <= count; t_val += 0.01)
   {
      // Calculate (x,y) coordinate
      float x_val = 0;
      float y_val = 0;
      for (int index = 0; index <= count; index++)
      {
         float basis = 1;
         for (int t = 0; t <= count; t++)
            if (index != t)
               basis *= (t_val - t) / (index - t);

         x_val += Px[index] * basis;
         y_val += Py[index] * basis;
      }

      // Draw line segment
      glVertex2f(x_val, y_val);
   }
   glVertex2f(Px[count], Py[count]);
   glEnd();
   
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Lagrange");
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   TBA\n");
   printf("Mouse operations:\n");
   printf("   'mouse down' - start drawing line\n");
   printf("   'mouse motion' - draw rubber-band line\n");
   printf("   'mouse up' - finish drawing line\n");
   glutMainLoop();
   return 0;
}
