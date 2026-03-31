//---------------------------------------
// Program: render2.cpp
// Purpose: Render random convex polygons into
//          image buffer using scan-line algorithm.
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

// Point structure
struct Point
{
   int x,y;
   unsigned char r,g,b;
};

// Global constants
#define XDIM 800
#define YDIM 800
#define MIN_COUNT 3
#define MAX_COUNT 7

// Global variables
unsigned char image[YDIM][XDIM][3];
Point scan_point[YDIM][2];
int scan_count[YDIM];
Point pt[MAX_COUNT];
int point_count = MIN_COUNT;
int polygon_count = MIN_COUNT;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);

   // Clear image
   for (int y = 0; y < YDIM; y++)
      for (int x = 0; x < XDIM; x++)
	 image[y][x][0] = image[y][x][1] = image[y][x][2] = 0;
}

//---------------------------------------
// Draw edge into scan line buffer
//---------------------------------------
void draw_edge(Point p1, Point p2)
{
   // Check point order
   if (p1.y > p2.y)
   {
      Point temp = p1;
      p1 = p2;
      p2 = temp;
   }

   // Calculate slope
   float dy = p2.y - p1.y;
   float dx = p2.x - p1.x;
   float dr = p2.r - p1.r;
   float dg = p2.g - p1.g;
   float db = p2.b - p1.b;
   float x = p1.x;
   float r = p1.r;
   float g = p1.g;
   float b = p1.b;

   // Interpolate points from p1 to p2
   for (int y=p1.y; y<p2.y; y++)
   {
      int pos = scan_count[y];
      scan_point[y][pos].y = y;
      scan_point[y][pos].x = (int)(x+0.5);
      scan_point[y][pos].r = (int)(r+0.5);
      scan_point[y][pos].g = (int)(g+0.5);
      scan_point[y][pos].b = (int)(b+0.5);
      if (scan_count[y] < 2) 
         scan_count[y]++;
      x = x + dx/dy;
      r = r + dr/dy;
      g = g + dg/dy;
      b = b + db/dy;
   }
}

//---------------------------------------
// Draw line into image buffer
//---------------------------------------
void draw_line(Point p1, Point p2)
{
   // Check point order
   if (p1.x > p2.x)
   {
      Point temp = p1;
      p1 = p2;
      p2 = temp;
   }

   // Calculate slope
   float dx = p2.x - p1.x;
   float dr = p2.r - p1.r;
   float dg = p2.g - p1.g;
   float db = p2.b - p1.b;
   float r = p1.r;
   float g = p1.g;
   float b = p1.b;

   // Interpolate points from p1 to p2
   int y = p1.y;
   for (int x=p1.x; x<p2.x; x++)
   {
      image[y][x][0] = (int)(r+0.5);
      image[y][x][1] = (int)(g+0.5);
      image[y][x][2] = (int)(b+0.5);
      r = r + dr/dx;
      g = g + dg/dx;
      b = b + db/dx;
   }
}

//---------------------------------------
// Render polygon in image
//---------------------------------------
void render_polygon(Point pt[], int count)
{
   // Clear scan line buffer
   for (int y = 0; y < YDIM; y++)
   {
      scan_count[y] = 0;
      scan_point[y][0].x = scan_point[y][0].y = -1;
      scan_point[y][0].r = scan_point[y][0].g = scan_point[y][0].b = 0;
      scan_point[y][1].x = scan_point[y][1].y = -1;
      scan_point[y][1].r = scan_point[y][1].g = scan_point[y][1].b = 0;
   } 

   // Draw edges into scan line buffer
   for (int i=0; i<count; i++)
      draw_edge(pt[i], pt[(i+1)%count]);

   // Draw lines into image buffer
   for (int y=0; y<YDIM; y++)
   {
      if (scan_count[y] == 2)
         draw_line(scan_point[y][0], scan_point[y][1]);
      else if (scan_count[y] == 1)
         draw_line(scan_point[y][0], scan_point[y][0]);
   }
}

//---------------------------------------
// Get distance from point to line
//---------------------------------------
int get_distance(Point p1, Point p2, Point p3)
{
   // Get line equation from p1 to p2
   int A = p1.y - p2.y;
   int B = p2.x - p1.x;
   int C = A * p1.x + B * p1.y; 

   // Calculate distance to p3
   int distance = A * p3.x + B * p3.y - C;
   return distance;
}

//---------------------------------------
// Check if polygon is convex
//---------------------------------------
bool convex_polygon(Point pt[], int count)
{
   // Get first distance value 
   int dist1 = get_distance(pt[0], pt[1], pt[2]);

   // Check all edges in the polygon
   for (int index=0; index<count; index++)
      for (int j=2; j<count; j++)
      {
         // Get distance from this point to this edge
         int index2 = (index+1)%count;
         int index3 = (index+j)%count;
         int dist2 = get_distance(pt[index], pt[index2], pt[index3]);

         // Check that distances have the same sign
         if ((dist1 < 0 && dist2 >= 0) || (dist2 < 0 && dist1 >= 0)) 
            return false;
      }

   // Polygon is convex
   return true;
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Initialize
   init();

   // Create convex polygon
   for (int i=0; i<polygon_count; i++)
   {
      bool convex = false;
      while (!convex)
      {
         // Create random points
         for (int p=0; p<point_count; p++)
         {
            // Define random coordinates and colors
            pt[p].x = rand() % XDIM;
            pt[p].y = rand() % YDIM;
            pt[p].r = rand() % 256;
            pt[p].g = rand() % 256;
            pt[p].b = rand() % 256;
         }
         convex = convex_polygon(pt, point_count);
      }

      // Render convex polygon
      render_polygon(pt, point_count);
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
   if ((key == '+') && (point_count < MAX_COUNT))
      point_count++;
   else if ((key == '-') && (point_count > MIN_COUNT))
      point_count--;
    printf("point count = %d\n", point_count);
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
    printf("   '+' - increase number of points on polygon \n");
    printf("   '-' - decrease number of points on polygon \n");
   glutMainLoop();
   return 0;
}
