//---------------------------------------
// Program: bitmap.cpp
// Purpose: Demonstrate use of bitmaps for 
//          displaying patterns and text.
// Author:  John Gauch
// Date:    April 2010
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
using namespace std;

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
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

//---------------------------------------
// Display message on screen
// Font choices include
// GLUT_BITMAP_TIMES_ROMAN_10 (or 24)
// GLUT_BITMAP_HELVETICA_10 (or 12 or 18)
//---------------------------------------
void display_message(void *font, string message)
{
   for (int i=0; i<int(message.length()); i++)
      glutBitmapCharacter(font, message[i]);
}

//---------------------------------------
// Display integer on screen
// Font choices include
// GLUT_BITMAP_TIMES_ROMAN_10 (or 24)
// GLUT_BITMAP_HELVETICA_10 (or 12 or 18)
//---------------------------------------
void display_integer(void *font, int num)
{
   stringstream s;
   s << num;
   display_message(font, s.str());
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Clear screen
   glClear(GL_COLOR_BUFFER_BIT);

   // Define checker board bitmap
   GLubyte wb[2] = {0x00, 0xff};
   GLubyte bitmap1[512];
   for (int i=0;i<64;i++)
   for (int j=0;j<8;j++)
      bitmap1[i*8+j] = wb[(i/8+j)%2];

   // Define vertical stripe bitmap
   GLubyte bitmap2[512];
   for (int i=0;i<64;i++)
   for (int j=0;j<8;j++)
      bitmap2[i*8+j] = wb[(i/8)%2];

   // Define horizontal stripe bitmap
   GLubyte bitmap3[512];
   for (int i=0;i<64;i++)
   for (int j=0;j<8;j++)
      bitmap3[i*8+j] = wb[j%2];

   // Display three bitmaps
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-0.2, -0.3, 0.0);
   glBitmap(64, 64, 0.0, 0.0, 0.0, 0.0, bitmap1);

   glColor3f(0.0, 1.0, 0.0);
   glRasterPos3f(0.2, 0.0, 0.0);
   glBitmap(64, 64, 0.0, 0.0, 64.0, 64.0, bitmap2);

   glColor3f(0.0, 0.0, 1.0);
   glRasterPos3f(0.6, 0.3, 0.0);
   glBitmap(64, 64, 0.0, 0.0, 0.0, 0.0, bitmap3);

   // Display characters
   glColor3f(1.0, 1.0, 0.0);
   glRasterPos3f(-0.6, -0.2, 0.0);
   glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '4');
   glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');

   // Display number
   glColor3f(0.0, 1.0, 1.0);
   glRasterPos3f(-0.6, -0.4, 0.0);
   display_integer(GLUT_BITMAP_HELVETICA_18, 17);

   // Display message
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-0.8, -0.6, 0.0);
   display_message(GLUT_BITMAP_HELVETICA_18, "Hello Mom");

   // Display screen
   glRasterPos3f(0.0, 0.0, 0.0);
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
   glutCreateWindow("Bitmap");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
