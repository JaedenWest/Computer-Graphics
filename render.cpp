//---------------------------------------
// Program: render.cpp
// Purpose: Render random colored rectangles
//          directly into image buffer.
// Author:  John Gauch
// Date:    Spring 2012
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
#define XDIM 800
#define YDIM 800
unsigned char image[YDIM][XDIM][3];
int rectangle_count = 5;
const int MIN_COUNT = 2;
const int MAX_COUNT = 10;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
}

//---------------------------------------
// Render rectangle in image
//---------------------------------------
void render_rectangle(
   int xmin, int xmax, int ymin, int ymax,
   int R, int G, int B)
{
   if (xmin > xmax)
      render_rectangle(xmax, xmin, ymin, ymax, R, G, B);
   else if (ymin > ymax)
      render_rectangle(xmin, xmax, ymax, ymin, R, G, B);
   else
   {
      // Fill region with specified color
      for (int y = ymin; y <= ymax; y++)
      for (int x = xmin; x <= xmax; x++)
      {
	 	 image[y][x][0] = R;
         image[y][x][1] = G;
         image[y][x][2] = B;
      }
   }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Clear image
   for (int y = 0; y < YDIM; y++)
      for (int x = 0; x < XDIM; x++)
	 image[y][x][0] = image[y][x][1] = image[y][x][2] = 0;

   // Render rectangles
   for (int i=0; i<rectangle_count; i++)
   {
      int xmin = rand() % XDIM;
      int xmax = rand() % XDIM;
      int ymin = rand() % YDIM;
      int ymax = rand() % YDIM;
      int R = rand() % 256;
      int G = rand() % 256;
      int B = rand() % 256;
      render_rectangle(xmin, xmax, ymin, ymax, R, G, B);
   }

   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   if ((key == '+') && (rectangle_count < MAX_COUNT))
      rectangle_count++;
   else if ((key == '-') && (rectangle_count > MIN_COUNT))
      rectangle_count--;
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Render");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
    printf("Keyboard commands:\n");
    printf("   '+' - increase number of rectanges drawn \n");
    printf("   '-' - decrease number of rectanges drawn \n");
   glutMainLoop();
   return 0;
}
