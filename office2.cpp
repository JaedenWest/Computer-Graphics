//---------------------------------------
// Program: office2.cpp
// Purpose: Demonstrate x,y rotations of office
//          furniture using keyboard callbacks.
// Author:  John Gauch
// Date:    September 2008
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
int xangle = 10;
int yangle = 15;
char mode = 'R';
int selected = 0;

// Define initial position and size of objects
#define NUM_OBJECTS 4
char object[NUM_OBJECTS] = {'t', 't', 'b', 'b'};
float position[NUM_OBJECTS][6] = {
   {0.5, 0.5, 0.5}, 
   {-0.5, -0.5, -0.5},
   {0.5, -0.5, 0.5},
   {-0.5, 0.5, -0.5} };
float size[NUM_OBJECTS][6] = {
   {0.5, 0.5, 0.6}, 
   {0.8, 0.5, 0.7},
   {0.5, 1.2, 0.2},
   {0.5, 1.2, 0.2} };
 
//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Function to draw 3D block
//---------------------------------------
void block(float xmin, float ymin, float zmin,
   float xmax, float ymax, float zmax, int selected)
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

   // Define display mode
   GLenum mode = (selected) ?  GL_LINE_LOOP : GL_POLYGON; 

   // Draw 6 faces
   glBegin(mode);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(bx, by, bz);
   glVertex3f(cx, cy, cz);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(mode);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(dx, dy, dz);
   glVertex3f(hx, hy, hz);
   glVertex3f(ex, ey, ez);
   glEnd();

   glBegin(mode);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(ax, ay, az);
   glVertex3f(ex, ey, ez);
   glVertex3f(fx, fy, fz);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(mode);
   glColor3f(0.0, 1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(fx, fy, fz);
   glVertex3f(ex, ey, ez);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(mode);
   glColor3f(1.0, 0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(cx, cy, cz);
   glVertex3f(bx, by, bz);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(mode);
   glColor3f(1.0, 1.0, 0.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(hx, hy, hz);
   glVertex3f(dx, dy, dz);
   glVertex3f(cx, cy, cz);
   glEnd();
}

//---------------------------------------
// Function to draw 3D book case
//---------------------------------------
void book_case(float xpos, float ypos, float zpos,
   float xsize, float ysize, float zsize, int selected)
{
   // Define six corners of book case
   float xmin = xpos - xsize / 2;
   float xmax = xpos + xsize / 2;
   float ymin = ypos - ysize / 2;
   float ymax = ypos + ysize / 2;
   float zmin = zpos - zsize / 2;
   float zmax = zpos + zsize / 2;
   float shelf = (xsize + ysize) / 40;

   // Draw back
   block(xmin, ymin, zmax - shelf, xmax, ymax, zmax, selected);

   // Draw sides
   block(xmin, ymin, zmin, xmin + shelf, ymax, zmax - shelf, selected);
   block(xmax - shelf, ymin, zmin, xmax, ymax, zmax - shelf, selected);
   block(xmin + shelf, ymin, zmin, xmax - shelf, ymin + shelf, zmax - shelf, selected);
   block(xmin + shelf, ymax - shelf, zmin, xmax - shelf, ymax, zmax - shelf, selected);

   // Draw shelves
   int count;
   for (count = 1; count < 4; count++)
   {
      float ystart = count * (ysize - shelf) / 4;
      block(xmin + shelf, ystart + ymin, zmin,
            xmax - shelf, ystart + ymin + shelf, zmax - shelf, selected);
   }
}

//---------------------------------------
// Function to draw 3D table
//---------------------------------------
void table(float xpos, float ypos, float zpos,
   float xsize, float ysize, float zsize, int selected)
{
   // Define six corners of table
   float xmin = xpos - xsize / 2;
   float xmax = xpos + xsize / 2;
   float ymin = ypos - ysize / 2;
   float ymax = ypos + ysize / 2;
   float zmin = zpos - zsize / 2;
   float zmax = zpos + zsize / 2;
   float leg = (xsize + ysize) / 20;

   // Draw top
   block(xmin, ymax - leg, zmin, xmax, ymax, zmax, selected);

   // Draw legs
   block(xmin, ymin, zmin, xmin + leg, ymax - leg, zmin + leg, selected);
   block(xmin, ymin, zmax - leg, xmin + leg, ymax - leg, zmax, selected);
   block(xmax - leg, ymin, zmin, xmax, ymax - leg, zmin + leg, selected);
   block(xmax - leg, ymin, zmax - leg, xmax, ymax - leg, zmax, selected);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Select object
   if ((key == 'n') || (key == 'N'))
      selected = (selected + 1) % NUM_OBJECTS; 
   else if ((key == 'p') || (key == 'P'))
      selected = (selected + NUM_OBJECTS - 1) % NUM_OBJECTS; 

   // Check for mode change
   if ((key == 'r') || (key == 'R'))
      mode = 'R';
   else if ((key == 't') || (key == 'T'))
      mode = 'T';
   else if ((key == 's') || (key == 'S'))
      mode = 'S';

   // Update angles
   if (mode == 'R')
   {
      if (key == 'x')
         xangle -= 5;
      else if (key == 'y')
         yangle -= 5;
      else if (key == 'X')
         xangle += 5;
      else if (key == 'Y')
         yangle += 5;
   }

   // Update position
   if (mode == 'T')
   {
      if (key == 'x')
         position[selected][0] -= 0.05;
      else if (key == 'y')
         position[selected][1] -= 0.05;
      else if (key == 'z')
         position[selected][2] -= 0.05;
      else if (key == 'X')
         position[selected][0] += 0.05;
      else if (key == 'Y')
         position[selected][1] += 0.05;
      else if (key == 'Z')
         position[selected][2] += 0.05;
   }

   // Update size
   if (mode == 'S')
   {
      if (key == 'x')
         size[selected][0] -= 0.05;
      else if (key == 'y')
         size[selected][1] -= 0.05;
      else if (key == 'z')
         size[selected][2] -= 0.05;
      else if (key == 'X')
         size[selected][0] += 0.05;
      else if (key == 'Y')
         size[selected][1] += 0.05;
      else if (key == 'Z')
         size[selected][2] += 0.05;
   }

   // Redraw objects
   glutPostRedisplay();
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

   // Draw all objects
   int i;
   for (i=0; i<4; i++)
   {
      // Draw table
      if (object[i] == 't')
         table(position[i][0], position[i][1], position[i][2], 
            size[i][0], size[i][1], size[i][2], i==selected);
 
      // Draw book case
      else if (object[i] == 'b')
         book_case(position[i][0], position[i][1], position[i][2], 
            size[i][0], size[i][1], size[i][2], i==selected);
   }
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
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Office");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   printf("Keyboard commands:\n");
   printf("   'x' - rotate x-axis -5 degrees\n");
   printf("   'X' - rotate x-axis +5 degrees\n");
   printf("   'y' - rotate y-axis -5 degrees\n");
   printf("   'Y' - rotate y-axis +5 degrees\n");
   glutMainLoop();
   return 0;
}
