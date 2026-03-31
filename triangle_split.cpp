//---------------------------------------
// Program: triangle_split.cpp
// Purpose: Generate random triangle mesh
//          using recursive splitting.
// Author:  John Gauch
// Date:    Februrary 2024
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

float rough = 0.1;
float depth = 2;

//---------------------------------------
// Calculate random value between [-R..R]
//---------------------------------------
float myrand(float R)
{
   return (2 * R * rand()) / RAND_MAX - R;
}

//---------------------------------------
// Recursive function to split triangles
//---------------------------------------
void split(float x1, float y1, 
           float x2, float y2, 
           float x3, float y3, 
           int depth, float rough)
{
   // Check terminating condition
   if (depth == 0)
   {
      // Draw triangle
      float red = (random() % 11) * 0.1;
      float green = (random() % 11) * 0.1;
      float blue = (random() % 11) * 0.1;
      glColor3f(red, green, blue);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
      glVertex2f(x3, y3);
   }

   // Handle recursive case
   else
   {
      // Generate midpoint with random displacement
      float len12 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
      float x12 = (x1 + x2) / 2 + myrand(len12*rough);
      float y12 = (y1 + y2) / 2 + myrand(len12*rough);

      // Generate midpoint with random displacement
      float len13 = sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1));
      float x13 = (x1 + x3) / 2 + myrand(len13*rough);
      float y13 = (y1 + y3) / 2 + myrand(len13*rough);

      // Generate midpoint with random displacement
      float len23 = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
      float x23 = (x2 + x3) / 2 + myrand(len23*rough);
      float y23 = (y2 + y3) / 2 + myrand(len23*rough);

      // Perform recursive calls
      split(x1, y1, x12, y12, x13, y13, depth-1, rough);
      split(x2, y2, x23, y23, x12, y12, depth-1, rough);
      split(x3, y3, x13, y13, x23, y23, depth-1, rough);
      split(x12,y12,x23, y23, x13, y13, depth-1, rough);
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
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_TRIANGLES);
   split(0.0, 0.7, 0.7, -0.2, -0.7, -0.2, depth, rough);
   glEnd();
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Adjust depth value
   if ((key == 'd') && (depth > 1)) depth--;
   if ((key == 'D') && (depth < 9)) depth++;

   // Adjust rough value
   if ((key == 'r') && (rough > 0.02)) rough-=0.01;
   if ((key == 'R') && (rough < 0.15)) rough+=0.01;

   // Quit program
   if (key == 'q') exit(0);

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
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Triangle-Split");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   printf("Keyboard commands:\n");
   printf("   'd' - decrease recursion depth\n");
   printf("   'D' - increase recursion depth\n");
   printf("   'r' - decrease surface roughness\n");
   printf("   'R' - increase surface roughness\n");
   printf("   'q' - quit program\n");
   glutMainLoop();
   return 0;
}
