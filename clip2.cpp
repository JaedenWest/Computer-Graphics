//---------------------------------------
// Program: clip2.cpp
// Purpose: Demonstrate Cohen-Sutherland 
//          line clipping algorthm.
// Author:  John Gauch
// Date:    March 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

// Clipping window coordinates
const float xmin = 20;
const float xmax = 80;
const float ymin = 20;
const float ymax = 80;
int line_count = 100;
int intersect_count = 0;

//---------------------------------------
// clip against x=xmin line
//---------------------------------------
bool clip_xmin(float &x1, float &y1, float &x2, float &y2)
{
   intersect_count++;
   float dx = (x2-x1);
   float dy = (y2-y1);

   // both points outside clip window
   if ((x1 < xmin) && (x2 < xmin))
      return false;

   // both points inside clip window
   else if ((x1 >= xmin) && (x2 >= xmin))
      return true;

   // x1 outside, x2 inside
   else if ((x1 < xmin) && (x2 >= xmin))
   {
      y1 = ((xmin-x1)*dy)/dx+y1;
      x1 = xmin;
   }

   // x1 inside, x2 outside
   else if ((x1 >= xmin) && (x2 < xmin))
   {
      y2 = ((xmin-x1)*dy)/dx+y1;
      x2 = xmin;
   }
   return true;
}

//---------------------------------------
// clip against x=xmax line
//---------------------------------------
bool clip_xmax(float &x1, float &y1, float &x2, float &y2)
{
   intersect_count++;
   float dx = (x2-x1);
   float dy = (y2-y1);

   // both points outside clip window
   if ((x1 >= xmax) && (x2 >= xmax))
      return false;

   // both points inside clip window
   else if ((x1 < xmax) && (x2 < xmax))
      return true;

   // x1 outside, x2 inside
   else if ((x1 >= xmax) && (x2 < xmax))
   {
      y1 = ((xmax-x1)*dy)/dx+y1;
      x1 = xmax;
   }

   // x1 inside, x2 outside
   else if ((x1 < xmax) && (x2 >= xmax))
   {
      y2 = ((xmax-x1)*dy)/dx+y1;
      x2 = xmax;
   }
   return true;
}

//---------------------------------------
// clip against y=ymin line
//---------------------------------------
bool clip_ymin(float &x1, float &y1, float &x2, float &y2)
{
   intersect_count++;
   float dx = (x2-x1);
   float dy = (y2-y1);

   // both points outside clip window
   if ((y1 < ymin) && (y2 < ymin))
      return false;

   // both points inside clip window
   else if ((y1 >= ymin) && (y2 >= ymin))
      return true;

   // y1 outside, y2 inside
   else if ((y1 < ymin) && (y2 >= ymin))
   {
      x1 = ((ymin-y1)*dx)/dy+x1;
      y1 = ymin;
   }

   // y1 inside, y2 outside
   else if ((y1 >= ymin) && (y2 < ymin))
   {
      x2 = ((ymin-y1)*dx)/dy+x1;
      y2 = ymin;
   }
   return true;
}

//---------------------------------------
// clip against y=ymax line
//---------------------------------------
bool clip_ymax(float &x1, float &y1, float &x2, float &y2)
{
   intersect_count++;
   float dx = (x2-x1);
   float dy = (y2-y1);

   // both points outside clip window
   if ((y1 >= ymax) && (y2 >= ymax))
      return false;

   // both points inside clip window
   else if ((y1 < ymax) && (y2 < ymax))
      return true;

   // y1 outside, y2 inside
   else if ((y1 >= ymax) && (y2 < ymax))
   {
      x1 = ((ymax-y1)*dx)/dy+x1;
      y1 = ymax;
   }

   // y1 inside, y2 outside
   else if ((y1 < ymax) && (y2 >= ymax))
   {
      x2 = ((ymax-y1)*dx)/dy+x1;
      y2 = ymax;
   }
   return true;
}

//---------------------------------------
// Cohen-Sutherland clipping
//---------------------------------------
bool clip(float &x1, float &y1, float &x2, float &y2)
{
   // define outcodes
   const int TOP = 1;
   const int BOTTOM = 2;
   const int RIGHT = 4;
   const int LEFT = 8;

   // calculate (x1,y1) outcode
   int code1 = 0;
   if (y1 > ymax) code1 += TOP; 
   if (y1 < ymin) code1 += BOTTOM; 
   if (x1 > xmax) code1 += RIGHT; 
   if (x1 < xmin) code1 += LEFT; 

   // calculate (x2,y2) outcode
   int code2 = 0;
   if (y2 > ymax) code2 += TOP; 
   if (y2 < ymin) code2 += BOTTOM; 
   if (x2 > xmax) code2 += RIGHT; 
   if (x2 < xmin) code2 += LEFT; 

   // combine outcodes
   int code_or = code1 | code2;
   int code_and = code1 & code2;

   // draw line
   if (code_or == 0) return true;

   // discard line
   if (code_and > 0) return false;

   // handle intersection cases
   bool result = true;
   if (result && (code_or & TOP) > 0)
      result = result && clip_ymax(x1, y1, x2, y2);
   if (result && (code_or & BOTTOM) > 0)
      result = result && clip_ymin(x1, y1, x2, y2);
   if (result && (code_or & RIGHT) > 0)
      result = result && clip_xmax(x1, y1, x2, y2);
   if (result && (code_or & LEFT) > 0)
      result = result && clip_xmin(x1, y1, x2, y2);
   return result;
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
   glOrtho(0, 100, 0, 100, 0, 100);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw clipping window
   glBegin(GL_LINE_LOOP);
   glColor3f(0.0, 0.0, 1.0);
   glVertex2i(xmin, ymin);
   glVertex2i(xmax, ymin);
   glVertex2i(xmax, ymax);
   glVertex2i(xmin, ymax);
   glEnd();

   // Draw random lines
   for (int i=0; i<line_count; i++)
   {
      // Get line endpoints
      float x1 = rand() % 100;
      float y1 = rand() % 100;
      float x2 = rand() % 100;
      float y2 = rand() % 100;

      // Draw unclipped line
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
      glEnd();

      // Draw clipped line
      if (clip(x1, y1, x2, y2))
      {
         glBegin(GL_LINES);
         glColor3f(0.0, 1.0, 0.0);
         glVertex2f(x1, y1);
         glVertex2f(x2, y2);
         glEnd();
      }
   }
   glFlush();

   // Print clipping information
   printf("Lines: %d, Intersections: %d\n", line_count, intersect_count); 
   intersect_count = 0;
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   if ((key == '+') && (line_count < 1000))
      line_count += 5;
   else if ((key == '-') && (line_count > 5))
      line_count -= 5;

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
   glutInitWindowPosition(100, 100);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Clip");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   printf("Keyboard commands:\n");
   printf("   '+' - increase line count by 5\n");
   printf("   '-' - decrease line count by 5\n");
   init();
   glutMainLoop();
   return 0;
}
