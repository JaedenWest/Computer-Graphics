//---------------------------------------
// Program: show_poly.cpp
// Purpose: To read polygons from input.txt 
//          and display on screen with normals.
// Author:  John Gauch
// Date:    Spring 2021
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <fstream>
#include <iostream>
using namespace std;

// Global constants
#define MIN_X_VIEW -3
#define MAX_X_VIEW 3
#define MIN_Y_VIEW -3 
#define MAX_Y_VIEW 3
#define MIN_Z_VIEW -3 
#define MAX_Z_VIEW 3
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500

// Global variables
float xangle = 5;
float yangle = 5;
float zangle = 5;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);
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
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 1.0, 0.0);

   // Open input.txt file
   ifstream din;
   din.open("input.txt");
   
   // Read and display sequence of rectangles
   while (!din.eof())
   {
      // Rectangle variables
      string word;
      float R[4], G[4], B[4];
      float Nx[4], Ny[4], Nz[4];
      float Px[4], Py[4], Pz[4];

      // Read four points of rectangle
      for (int i=0; i<4; i++)
      {
         if ((din >> word) && (word == "color"))
             din >> R[i] >> G[i] >> B[i];
         if ((din >> word) && (word == "normal"))
             din >> Nx[i] >> Ny[i] >> Nz[i];
         if ((din >> word) && (word == "point"))
             din >> Px[i] >> Py[i] >> Pz[i];
      }      

      // Check for end of file
      if (din.eof())
         continue;

      // Display rectangle interior
      glBegin(GL_POLYGON); 
      for (int i=0; i<4; i++)
      {
         glColor3f(R[i], G[i], B[i]);
         glVertex3f(Px[i], Py[i], Pz[i]);
      }
      glEnd();

      // Display rectangle outline
      glLineWidth(5);
      glBegin(GL_LINE_LOOP); 
      glColor3f(1,1,1);
      for (int i=0; i<4; i++)
         glVertex3f(Px[i], Py[i], Pz[i]);
      glEnd();

      // Display rectangle normals
      glLineWidth(5);
      glBegin(GL_LINES); 
      glColor3f(1,1,1);
      for (int i=0; i<4; i++)
      {
         glVertex3f(Px[i], Py[i], Pz[i]);
         glVertex3f(Px[i]+Nx[i]/2, Py[i]+Ny[i]/2, Pz[i]+Nz[i]/2);
      }
      glEnd();
   }

   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'x')
      xangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'y') 
      yangle -= 5;
   else if (key == 'Y') 
      yangle += 5;
   else if (key == 'z') 
      zangle -= 5;
   else if (key == 'Z') 
      yangle += 5;
   
   // Redraw objects
   glutPostRedisplay();
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
   glutCreateWindow("Show Poly");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   glutMainLoop();
   return 0;
}
