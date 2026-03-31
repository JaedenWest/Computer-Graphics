//---------------------------------------
// Program: maze.cpp
// Purpose: Create an interactive maze with
//          texure mapped blocks.
// Author:  John Gauch
// Date:    April 2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "../libim/im_color.h"
#include <iostream>
#include <fstream>
using namespace std;

// Global variables 
#define SIZE 1024
int xangle = 0;
int yangle = 0;
int zangle = 0;

// Maze variables
const int MAX_ROWS = 100;
const int MAX_COLS = 100;
char maze[MAX_ROWS][MAX_COLS];
int player_row = 0;
int player_col = 0;
int rows = 0;
int cols = 0;

// Texture variables
unsigned char *brick_texture = NULL;
unsigned char *fire_texture = NULL;
unsigned char *rock_texture = NULL;
unsigned char *wood_texture = NULL;
unsigned char *grass_texture = NULL;
int xdim = 0;
int ydim = 0;

//---------------------------------------
// Read maze file
//---------------------------------------
void read_maze()
{
   // Initialize maze
   for (int r=0; r<MAX_ROWS; r++)
      for (int c=0; c<MAX_COLS; c++)
         maze[r][c] = ' ';

   // Open input file
   ifstream din;
   string filename = "maze.txt";
   din.open(filename);
   if (din.fail())
   {
      cout << "Error: could not open " << filename << endl;
      exit(-1);
   }

   // Read header
   din >> rows >> cols;
   din >> player_row >> player_col;
   string line;
   getline(din, line);

   // Read maze
   for (int r=0; r<rows; r++)
   {
      getline(din, line);
      for (int c=0; c<cols && c<line.length(); c++)
         maze[r][c] = line[c];
   }

   // Error checking
   if (maze[player_row][player_col] != ' ')
   {
      cout << "Error: starting position was not empty" << endl;
      exit(-1);
   }
}

//---------------------------------------
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);
   // printf("Reading image %s\n", name);
   xdim = 1; while (xdim < image.R.Xdim) xdim*=2; xdim /=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2; ydim /=2;
   image.Interpolate(xdim, ydim);
   // printf("Interpolating to %d by %d\n", xdim, ydim);

   // Copy image into texture array
   texture = (unsigned char *)malloc((unsigned int)(xdim*ydim*3));
   int index = 0;
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++)
      {
         texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
      }
}

//---------------------------------------
// Function to draw 3D block
//---------------------------------------
void block(float xmin, float ymin, float zmin,
           float xmax, float ymax, float zmax)
{
   // Define 8 vertices
   float ax = xmin, ay = ymin, az = zmax;
   float bx = xmax, by = ymin, bz = zmax;
   float cx = xmax, cy = ymax, cz = zmax;
   float dx = xmin, dy = ymax, dz = zmax;

   float ex = xmin, ey = ymin, ez = zmin;
   float fx = xmax, fy = ymin, fz = zmin;
   float gx = xmax, gy = ymax, gz = zmin;
   float hx = xmin, hy = ymax, hz = zmin;

   // Draw 6 faces
   glBegin(GL_POLYGON);  
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_POLYGON); 
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON); 
   glTexCoord2f(0.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glEnd();

   glBegin(GL_POLYGON); 
   glTexCoord2f(0.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(cx, cy, cz);
   glEnd();

   glBegin(GL_POLYGON); 
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_POLYGON); 
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Init maze
   read_maze();
   float radius = sqrt(rows*rows + cols*cols)/2;

   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-radius, radius, -radius, radius, -radius, radius);
   glEnable(GL_DEPTH_TEST);

   // Init texture
   init_texture((char *)"brick.jpg", brick_texture, xdim, ydim);
   init_texture((char *)"fire.jpg", fire_texture, xdim, ydim);
   init_texture((char *)"rock.jpg", rock_texture, xdim, ydim);
   init_texture((char *)"wood.jpg", wood_texture, xdim, ydim);
   init_texture((char *)"grass.jpg", grass_texture, xdim, ydim);
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_texture);
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
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Draw grass background
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_texture);
   glBegin(GL_POLYGON); 
   glTexCoord2f(0, 0);
   glVertex3f(-cols/2, -rows/2, 0.01);
   glTexCoord2f(1, 0);
   glVertex3f( cols/2, -rows/2, 0.01);
   glTexCoord2f(1, 1);
   glVertex3f( cols/2,  rows/2, 0.01);
   glTexCoord2f(0, 1);
   glVertex3f(-cols/2,  rows/2, 0.01);
   glEnd();

   // Draw brick blocks
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, brick_texture);
   for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
         if (maze[r][c] == 'b')
            block(c-cols/2, r-rows/2, 0, c-cols/2+1, r-rows/2+1, 1);

   // Draw rock blocks
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, rock_texture);
   for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
         if (maze[r][c] == 'r')
            block(c-cols/2, r-rows/2, 0, c-cols/2+1, r-rows/2+1, 1);

   // Draw wood blocks
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, wood_texture);
   for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
         if (maze[r][c] == 'w')
            block(c-cols/2, r-rows/2, 0, c-cols/2+1, r-rows/2+1, 1);

   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Rotate scene
   if (key == 'x')
      xangle -= 5;
   else if (key == 'y')
      yangle -= 5;
   else if (key == 'z')
      zangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'Y')
      yangle += 5;
   else if (key == 'Z')
      zangle += 5;
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(SIZE, SIZE);
   glutInitWindowPosition(SIZE/2, SIZE/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Texture");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);

   init();
   printf("Rotation commands:\n");
   printf("   x - decrease x rotation\n");
   printf("   y - decrease y rotation\n");
   printf("   z - decrease z rotation\n");
   printf("   X - increase x rotation\n");
   printf("   Y - increase y rotation\n");
   printf("   Z - increase z rotation\n");
   glutMainLoop();
   return 0;
}
