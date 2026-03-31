//---------------------------------------
// Program: line_split.cpp
// Purpose: Generate random line sequences
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
#include <fstream>
#include <vector>
using namespace std;

float MIN_LENGTH = 0.02;
vector<float> xpos;
vector<float> ypos;

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
void split(float x1, float y1, float x2, float y2, float rough)
{
   // Check terminating condition
   float dx = x2 - x1;
   float dy = y2 - y1;
   float length = sqrt(dx * dx + dy * dy);
   if (length <= MIN_LENGTH)
   {
      // Draw next point in line strip
      glVertex2f(x2, y2);
      xpos.push_back(x2);
      ypos.push_back(y2);
   }

   // Handle recursive case
   else
   {
      // Generate midpoint with random displacement
      float xmid = (x2 + x1) / 2 + myrand(length*rough);
      float ymid = (y2 + y1) / 2 + myrand(length*rough);

      // Perform recursive calls
      split(x1, y1, xmid, ymid, rough);
      split(xmid, ymid, x2, y2, rough);
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

   // Draw red lines
   glBegin(GL_LINE_STRIP);
   glColor3f(1.0, 0.0, 0.0);
   glVertex2f(-0.5, -0.5);
   split(-0.5, -0.5, 0.5, 0.5, 0.3);
   glEnd();

   // Draw green lines
   glBegin(GL_LINE_STRIP);
   glColor3f(0.0, 1.0, 0.0);
   glVertex2f(0.5, -0.5);
   split(0.5, -0.5, -0.5, 0.5, 0.1);
   glEnd();
   glFlush();
   return;

   // Save lines
   ofstream dout;
   dout.open("lines.csv");
   for (int i=0; i<xpos.size(); i++)
      dout << xpos[i] << "," << ypos[i] << endl;
   dout.close();
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
