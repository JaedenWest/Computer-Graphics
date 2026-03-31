//---------------------------------------
// Program: bezier.cpp
// Purpose: Demonstrate geometric construction 
//          of Bezier curves.
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
int count = -1;
float Px[4], Py[4];
float Qx[3], Qy[3];
float Rx[2], Ry[2];
float stop_t = 0.5;

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
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update t stop value
   if ((key == '+') && (stop_t < 1.0))
      stop_t += 0.1;
   else if ((key == '-') && (stop_t > 0.0))
      stop_t -= 0.1;
   
   // Redraw objects
   glutPostRedisplay();
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
      if (count < 3) count++; 
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

   // Draw bezier curve
   if (count == 3)
   {
      glColor3f(1.0, 0.0, 0.0);
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);

      // Calculate points on curve
      for (float t=0; t<=stop_t; t+=0.01)
      {
         // Calculate Q points
         for (int i=0; i<3; i++)
         {
            Qx[i] = (1-t)*Px[i] + t*Px[i+1];
            Qy[i] = (1-t)*Py[i] + t*Py[i+1];
         }

         // Calculate R points
         for (int i=0; i<2; i++)
         {
            Rx[i] = (1-t)*Qx[i] + t*Qx[i+1];
            Ry[i] = (1-t)*Qy[i] + t*Qy[i+1];
         }

         // Calculate (x,y) point
         float x = (1-t)*Rx[0] + t*Rx[1];
         float y = (1-t)*Ry[0] + t*Ry[1];
         glVertex2f(x, y);
      }
      glEnd();

      // Display Q lines
      glColor3f(0.0, 1.0, 0.0);
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);
      for (int i=0; i<3; i++)
         glVertex2f(Qx[i], Qy[i]);
      glEnd();

      // Display R lines
      glColor3f(0.0, 0.0, 1.0);
      glLineWidth(2);
      glBegin(GL_LINE_STRIP);
      for (int i=0; i<2; i++)
         glVertex2f(Rx[i], Ry[i]);
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
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Bezier");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   '+' - increase length of bezier curve\n");
   printf("   '-' - decrease length of bezier curve\n");
   printf("Mouse operations:\n");
   printf("   'mouse down' - start drawing line\n");
   printf("   'mouse motion' - draw rubber-band line\n");
   printf("   'mouse up' - finish drawing line\n");
   glutMainLoop();
   return 0;
}
