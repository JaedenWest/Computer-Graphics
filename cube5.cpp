//---------------------------------------
// Program: cube5.cpp
// Purpose: Demonstrate glFrustum and 
//          gluLookAt functions.
// Author:  John Gauch
// Date:    February 2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLenum mode = GL_POLYGON;
float xpos = 0.1;
float ypos = 0.1;
float zpos = 1.1;
float step = 0.01;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Turn on Z buffer
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);

   // Define viewing frustum
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-3,3, -3,3, -3,3);

   // Define camera position
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluLookAt(xpos,ypos,zpos, 0,0,0, 0,1,0);
}

//---------------------------------------
// Function to draw 3D cube
//---------------------------------------
void cube(float midx, float midy, float midz, float size)
{
   // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

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
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Quit
   if (key == 'q')
      exit(0);

   // Update camera position
   else if (key == 'x')
      xpos -= step;
   else if (key == 'y')
      ypos -= step;
   else if (key == 'z')
      zpos -= step;

   else if (key == 'X')
      xpos += step;
   else if (key == 'Y')
      ypos += step;
   else if (key == 'Z')
      zpos += step;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Define camera position
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluLookAt(xpos,ypos,zpos, 0,0,0, 0,1,0);

   // Draw two cubes
   cube(0.1, 0.2, 0.3, 0.5);
   cube(0.3, 0.2, 0.1, 0.3);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Cube");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   glutMainLoop();
   return 0;
}
