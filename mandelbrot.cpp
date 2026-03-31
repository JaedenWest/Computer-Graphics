//---------------------------------------
// Program: mandelbrot.cpp
// Purpose: Generate and display grey-scale
//          Mandelbot set image.
// Author:  John Gauch
// Date:    November 2008
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
float center_re = -0.5;
float center_im = 0.0;
float radius = 1.0;

//---------------------------------------
// Calculate Z=Z^2+C for given C
//---------------------------------------
int mandelbrot(float C_re, float C_im, int max_count)
{
   int count = 0;
   float Z_re = 0;
   float Z_im = 0;
   float magnitude = 0;
   float max_magnitude = 1000000;

   // Calculate Z=Z^2+C for given C
   while ((magnitude < max_magnitude) && (count++ < max_count))
   {
      float N_re = Z_re * Z_re - Z_im * Z_im + C_re;
      float N_im = Z_re * Z_im + Z_im * Z_re + C_im;
      Z_re = N_re;
      Z_im = N_im;
      magnitude = Z_re * Z_re + Z_im * Z_im;
   }

   // Return number of iterations 
   if (count >= max_count)
      return 0;
   else
      return count;
}

//---------------------------------------
// Initialize mandelbrot image
//---------------------------------------
void init_mandelbrot()
{
   for (int y = 0; y < YDIM; y++)
      for (int x = 0; x < XDIM; x++)
      {
	 float C_re = center_re - radius + (2 * radius * x) / XDIM;
	 float C_im = center_im - radius + (2 * radius * y) / YDIM;
	 image[y][x][0] = image[y][x][1] = image[y][x][2] =
	    4 * mandelbrot(C_re, C_im, 64);
      }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   init_mandelbrot();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
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
   glutCreateWindow("Mandelbrot");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
