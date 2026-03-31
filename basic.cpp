//---------------------------------------
// Program: basic.cpp
// Purpose: Basic 2D graphics program.
// Author:  John Gauch
// Date:    Spring 2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Image for drawing 2D objects
#define XDIM 1000
#define YDIM 1000
unsigned char image[YDIM][XDIM][3];

//---------------------------------------
// Draw rectangle in image
//---------------------------------------
void draw_rectangle(
   int xmin, int xmax, int ymin, int ymax,
   int R, int G, int B)
{
   if (xmin > xmax)
      draw_rectangle(xmax, xmin, ymin, ymax, R, G, B);
   else if (ymin > ymax)
      draw_rectangle(xmin, xmax, ymax, ymin, R, G, B);
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
// Save image in P3 ppm file
//---------------------------------------
void save_image(string filename)
{
   ofstream dout(filename);
   dout << "P3\n";
   dout << XDIM << " " << YDIM << "\n255\n";

   for (int y = 0; y < YDIM; y++)
   for (int x = 0; x < XDIM; x++)
   {
      dout << (int)image[y][x][0] << " ";
      dout << (int)image[y][x][1] << " ";
      dout << (int)image[y][x][2] << endl;
   }
   dout << endl;
   dout.close();
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

   // Draw 2D objects on image
   draw_rectangle(100,300, 300,500, 255,0,255);
   draw_rectangle(400,700, 600,900, 255,255,0);
   draw_rectangle(750,550, 250,500, 0,0,255);
   draw_rectangle(50,150, 50,150, 0,255,255);
   // save_image("output.ppm");

   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
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
   glutCreateWindow("Basic");
   glClearColor(0.0, 0.0, 0.0, 1.0);

   // Specify callback function
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
