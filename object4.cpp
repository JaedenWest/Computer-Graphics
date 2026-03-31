//---------------------------------------
// Program: object.cpp
// Purpose: Use Phong shading to display
//          six objects of rotation.
// Author:  John Gauch
// Date:    March 2013
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Surface class
const int SIZE = 20;
class Surface 
{
public:
   float Px[SIZE][SIZE];
   float Py[SIZE][SIZE];
   float Pz[SIZE][SIZE];
   float Nx[SIZE][SIZE];
   float Ny[SIZE][SIZE];
   float Nz[SIZE][SIZE];
};

// Global variables 
float xangle = 0;
float yangle = 0;
float zangle = 0;
Surface sphere;
Surface cylinder;
Surface cone;
Surface donut;
Surface vase;
Surface mobius;

#include "shading.cpp"

//---------------------------------------
// Calculate surface normals
//---------------------------------------
void surface_normals(Surface &s)
{
   // Find surface normals
   for (int u = 0; u < SIZE-1; u++)
   for (int v = 0; v < SIZE-1; v++)
   {
      // Get two tangent vectors
      float Ux = s.Px[u+1][v] - s.Px[u][v];
      float Uy = s.Py[u+1][v] - s.Py[u][v];
      float Uz = s.Pz[u+1][v] - s.Pz[u][v];
      float Vx = s.Px[u][v+1] - s.Px[u][v];
      float Vy = s.Py[u][v+1] - s.Py[u][v];
      float Vz = s.Pz[u][v+1] - s.Pz[u][v];
      
      // Do cross product
      s.Nx[u][v] = Uy * Vz - Uz * Vy;
      s.Ny[u][v] = Uz * Vx - Ux * Vz;
      s.Nz[u][v] = Ux * Vy - Uy * Vx;
      float length = sqrt( 
         s.Nx[u][v] * s.Nx[u][v] + 
         s.Ny[u][v] * s.Ny[u][v] + 
         s.Nz[u][v] * s.Nz[u][v]);
      if (length > 0)
      {
         s.Nx[u][v] /= length;
         s.Ny[u][v] /= length;
         s.Nz[u][v] /= length;
      }
   } 

   // Wrap around values for last row and col
   for (int u = 0; u < SIZE; u++)
   {
      s.Nx[u][SIZE-1] = s.Nx[u][0];
      s.Ny[u][SIZE-1] = s.Ny[u][0];
      s.Nz[u][SIZE-1] = s.Nz[u][0];
   }
   for (int v = 0; v < SIZE; v++)
   {
      s.Nx[SIZE-1][v] = s.Nx[0][v];
      s.Ny[SIZE-1][v] = s.Ny[0][v];
      s.Nz[SIZE-1][v] = s.Nz[0][v];
   }
}

//---------------------------------------
// Initialize sphere surface and normals
//---------------------------------------
void init_sphere(Surface &s)
{
   // Initialize sphere surface and normals
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         float angle2 = v * M_PI / (SIZE - 1);
         s.Px[u][v] = s.Nx[u][v] = - cos(angle1) * sin(angle2);
         s.Py[u][v] = s.Ny[u][v] = - sin(angle1) * sin(angle2);
         s.Pz[u][v] = s.Nz[u][v] = - cos(angle2);
      }
   }
}

//---------------------------------------
// Initialize cylinder surface and normals
//---------------------------------------
void init_cylinder(Surface &s)
{
   // Initialize cylinder surface and normals
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         s.Px[u][v] = cos(angle1);
         s.Py[u][v] = sin(angle1);
         s.Pz[u][v] = 2.0 * v / (SIZE - 1.0) - 1.0;

         s.Nx[u][v] = cos(angle1);
         s.Ny[u][v] = sin(angle1);
         s.Nz[u][v] = 0;
      }
   }
}

//---------------------------------------
// Initialize cone surface and normals
//---------------------------------------
void init_cone(Surface &s)
{
   // Initialize cone surface and normals
   float root5 = sqrt(5.0);
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         float radius = v / (SIZE - 1.0);
         s.Px[u][v] = cos(angle1) * radius;
         s.Py[u][v] = sin(angle1) * radius;
         s.Pz[u][v] = 2.0 * v / (SIZE - 1.0) - 1.0;

         s.Nx[u][v] = cos(angle1) * 2.0 / root5;
         s.Ny[u][v] = sin(angle1) * 2.0 / root5;
         s.Nz[u][v] = -1.0 / root5;
      }
   }
}

//---------------------------------------
// Initialize donut surface and normals
//---------------------------------------
void init_donut(Surface &s)
{
   // Initialize donut surface and normals
   float radius1 = 0.25;
   float radius2 = 0.75;
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         float angle2 = 2 * v * M_PI / (SIZE - 1);
         s.Px[u][v] = cos(angle1) * (radius2 + radius1*cos(angle2));
         s.Py[u][v] = sin(angle1) * (radius2 + radius1*cos(angle2));
         s.Pz[u][v] = radius1 * sin(angle2);
      }
   }
   surface_normals(s);
}

//---------------------------------------
// Initialize vase surface and normals
//---------------------------------------
void init_vase(Surface &s)
{
   // Initialize vase surface and normals
   float radius1 = 0.25;
   float radius2 = 0.75;
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         float angle2 = 2 * v * M_PI / (SIZE - 1);
         s.Px[u][v] = cos(angle1) * (radius2 + radius1*sin(angle2));
         s.Py[u][v] = sin(angle1) * (radius2 + radius1*sin(angle2));
         s.Pz[u][v] = 2.0 * v / (SIZE - 1.0) - 1.0;
      }
   }
   surface_normals(s);
}

//---------------------------------------
// Initialize Mobius strip surface
//---------------------------------------
void init_mobius(Surface &s)
{
   // Initialize mobius surface 
   for (int u = 0; u < SIZE; u++)
   {
      float angle1 = 2 * u * M_PI / (SIZE - 1);
      for (int v = 0; v < SIZE; v++)
      {
         float width = v / (SIZE - 1.0) - 0.5;
         s.Px[u][v] = (1.0 + width * cos(angle1/2)) * cos(angle1);
         s.Py[u][v] = (1.0 + width * cos(angle1/2)) * sin(angle1);
         s.Pz[u][v] = width * sin(angle1/2);
      }
   }
   surface_normals(s);
}

//---------------------------------------
// Print surface coordinates and normals.
//---------------------------------------
void print_surface(Surface &s)
{
   for (int u = 0; u < SIZE; u++)
      for (int v = 0; v < SIZE; v++)
         printf("%d %d %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n", u, v, 
            s.Px[u][v], s.Py[u][v], s.Pz[u][v],
            s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
}

//---------------------------------------
// Display surface coordinates and normals.
//---------------------------------------
void display_surface(Surface &s, bool top_and_bottom)
{
   for (int u = 0; u < SIZE-1; u++)
      for (int v = 0; v < SIZE-1; v++)
      {
         glBegin(GL_POLYGON);
         glNormal3f(s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
         glVertex3f(s.Px[u][v], s.Py[u][v], s.Pz[u][v]);
         glNormal3f(s.Nx[u + 1][v], s.Ny[u + 1][v], s.Nz[u + 1][v]);
         glVertex3f(s.Px[u + 1][v], s.Py[u + 1][v], s.Pz[u + 1][v]);
         glNormal3f(s.Nx[u + 1][v + 1], s.Ny[u + 1][v + 1], s.Nz[u + 1][v + 1]);
         glVertex3f(s.Px[u + 1][v + 1], s.Py[u + 1][v + 1], s.Pz[u + 1][v + 1]);
         glNormal3f(s.Nx[u][v + 1], s.Ny[u][v + 1], s.Nz[u][v + 1]);
         glVertex3f(s.Px[u][v + 1], s.Py[u][v + 1], s.Pz[u][v + 1]);
         glEnd();
      }

   if (top_and_bottom)
   {
      // Top polygon
      glBegin(GL_POLYGON);
      glNormal3f(0, 0, -1);
      for (int u = SIZE-1; u >= 0; u--)
         glVertex3f(s.Px[u][0], s.Py[u][0], s.Pz[u][0]);
      glEnd();

      // Bottom polygon
      glBegin(GL_POLYGON);
      glNormal3f(0, 0, 1);
      for (int u = 0; u < SIZE; u++)
         glVertex3f(s.Px[u][SIZE-1], s.Py[u][SIZE-1], s.Pz[u][SIZE-1]);
      glEnd();
   }
}

//---------------------------------------
// Display surface coordinates and normals.
//---------------------------------------
void display_surface(Surface &s, bool top_and_bottom,
   float scale, float dx, float dy, float dz)
{
   glPushMatrix();
   glTranslatef(dx, dy, dz);
   glScalef(scale, scale, scale);
   display_surface(s, top_and_bottom);
   glPopMatrix();
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   float radius = 2;
   glOrtho(-radius, radius, -radius, radius, -radius, radius);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);

   // Initialize light source
   init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
   init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);

   // Initialize surfaces
   init_sphere(sphere);
   init_cylinder(cylinder);
   init_cone(cone);
   init_donut(donut);
   init_vase(vase);
   init_mobius(mobius);
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
      zangle += 5;

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
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Draw geometric objects
   init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);
   display_surface(sphere, false, 0.5, -1, 0, 0);
   init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.4, 0.6);
   display_surface(cylinder, true, 0.5, 1, 0, 0);
   init_material(Ka, Kd, Ks, 100 * Kp, 0.6, 0.8, 0.4);
   display_surface(cone, true, 0.5, 0, -1, 0);
   init_material(Ka, Kd, Ks, 100 * Kp, 0.6, 0.4, 0.8);
   display_surface(donut, false, 0.5, 0, 1, 0);
   init_material(Ka, Kd, Ks, 100 * Kp, 0.4, 0.8, 0.6);
   display_surface(vase, true, 0.5, 0, 0, -1);
   init_material(Ka, Kd, Ks, 100 * Kp, 0.4, 0.6, 0.8);
   display_surface(mobius, false, 0.5, 0, 0, 1);
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
   glutCreateWindow("Object");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   glutMainLoop();
   return 0;
}

