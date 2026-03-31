//---------------------------------------
// Program: office4.cpp
// Purpose: Demonstrate use of mouse callbacks
//          to rotate office furniture models
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
#define ROTATE 1
#define TRANSLATE 2
int mode = ROTATE;

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
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(bx, by, bz);
   glVertex3f(cx, cy, cz);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(dx, dy, dz);
   glVertex3f(hx, hy, hz);
   glVertex3f(ex, ey, ez);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(ax, ay, az);
   glVertex3f(ex, ey, ez);
   glVertex3f(fx, fy, fz);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(0.0, 1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(fx, fy, fz);
   glVertex3f(ex, ey, ez);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(cx, cy, cz);
   glVertex3f(bx, by, bz);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(GL_POLYGON);
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
	       float xsize, float ysize, float zsize)
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
   block(xmin, ymin, zmin, xmax, ymax, zmin + shelf);

   // Draw sides
   block(xmin, ymin, zmin, xmin + shelf, ymax, zmax);
   block(xmax - shelf, ymin, zmin, xmax, ymax, zmax);
   block(xmin + shelf, ymin, zmin, xmax - shelf, ymin + shelf, zmax);
   block(xmin + shelf, ymax - shelf, zmin, xmax - shelf, ymax, zmax);

   // Draw shelves
   int count;
   for (count = 1; count < 4; count++)
   {
      float ystart = count * (ysize - shelf) / 4;
      block(xmin + shelf, ystart + ymin, zmin,
		xmax - shelf, ystart + ymin + shelf, zmax);
   }
}

//---------------------------------------
// Function to draw 3D table
//---------------------------------------
void table(float xpos, float ypos, float zpos,
	   float xsize, float ysize, float zsize)
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
   block(xmin, ymin, zmax - leg, xmax, ymax, zmax);

   // Draw legs
   block(xmin, ymin, zmin, xmin + leg, ymin + leg, zmax - leg);
   block(xmin, ymax - leg, zmin, xmin + leg, ymax, zmax - leg);
   block(xmax - leg, ymax - leg, zmin, xmax, ymax, zmax - leg);
   block(xmax - leg, ymin, zmin, xmax, ymin + leg, zmax - leg);
}

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
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//---------------------------------------
// Translate model using premultiply
//---------------------------------------
void translate(float dx, float dy, float dz)
{
   float current[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, current);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(dx, dy, dz);
   glMultMatrixf(current);
}

//---------------------------------------
// Rotate model using premultiply
//---------------------------------------
void rotate(float angle, float ax, float ay, float az)
{
   float current[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, current);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(angle, ax, ay, az);
   glMultMatrixf(current);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Draw tables and book cases
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   book_case(0.5, 0.5, 0.5, 0.7, 0.5, 0.6);
   book_case(-0.5, -0.5, -0.5, 0.8, 0.6, 0.7);
   book_case(0.5, -0.5, 0.5, 0.4, 0.8, 0.2);
   book_case(-0.5, 0.5, -0.5, 0.5, 0.9, 0.2);
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Determine if we are in ROTATE or TRANSLATE mode
   if ((key == 'r') || (key == 'R'))
   {
      printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
      mode = ROTATE;
   }
   else if ((key == 't') || (key == 'T'))
   {
      printf("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
      mode = TRANSLATE;
   }

   // Handle ROTATE
   if (mode == ROTATE)
   {
      if (key == 'x')
	 rotate(-5, 1, 0, 0);
      else if (key == 'y')
	 rotate(-5, 0, 1, 0);
      else if (key == 'z')
	 rotate(-5, 0, 0, 1);
      else if (key == 'X')
	 rotate(5, 1, 0, 0);
      else if (key == 'Y')
	 rotate(5, 0, 1, 0);
      else if (key == 'Z')
	 rotate(5, 0, 0, 1);
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if (mode == TRANSLATE)
   {
      if (key == 'x')
         translate(-0.01, 0, 0);
      else if (key == 'y')
         translate(0, -0.01, 0);
      else if (key == 'z')
         translate(0, 0, -0.01);
      else if (key == 'X')
         translate(0.01, 0, 0);
      else if (key == 'Y')
         translate(0, 0.01, 0);
      else if (key == 'Z')
         translate(0, 0, 0.01);
      glutPostRedisplay();
   }
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Handle mouse down
   static int xdown, ydown;
   if (state == GLUT_DOWN)
   {
      xdown = x;
      ydown = y;
   }

   // Handle ROTATE
   if ((mode == ROTATE) && (state == GLUT_UP))
   {
      rotate(y-ydown, 1, 0, 0);
      rotate(x-xdown, 0, 1, 0);
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if ((mode == TRANSLATE) && (state == GLUT_UP))
   {
      translate((x-xdown)/500.0, (ydown-y)/500.0, 0);
      glutPostRedisplay();
   }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Office");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
    init();
    printf("Keyboard commands:\n");
    printf("   't' or 'T' - go to translate mode\n");
    printf("   'r' or 'R' - go to rotate mode\n");
    printf("   'x' - rotate or translate on x-axis -5\n");
    printf("   'X' - rotate or translate on x-axis +5\n");
    printf("   'y' - rotate or translate on y-axis -5\n");
    printf("   'Y' - rotate or translate on y-axis +5\n");
    printf("   'z' - rotate or translate on z-axis -5\n");
    printf("   'Z' - rotate or translate on z-axis +5\n");
    printf("Mouse operations:\n");
    printf("   'mouse down' - start selecting rotation direction\n");
    printf("   'mouse up' - finish selecting rotation direction\n");
   glutMainLoop();
   return 0;
}
