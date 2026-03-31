//---------------------------------------
// Program: object.cpp
// Purpose: Texture map earth from space
//          photograph onto objects of rotation.
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
#include "libim/im_color.h"

// Surface class
const int SIZE = 20;
class Surface 
{
public:
   // surface points
   float Px[SIZE][SIZE];
   float Py[SIZE][SIZE];
   float Pz[SIZE][SIZE];

   // surface normals
   float Nx[SIZE][SIZE];
   float Ny[SIZE][SIZE];
   float Nz[SIZE][SIZE];

   // texture map
   unsigned char *texture;
   int xdim, ydim;
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
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);
   // printf("Reading image %s\n", name);
   xdim = 1; while (xdim < image.R.Xdim) xdim*=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2;
   if (xdim - image.R.Xdim > image.R.Xdim - xdim/2) xdim /=2;
   if (ydim - image.R.Ydim > image.R.Ydim - ydim/2) ydim /=2;
   // printf("Interpolating to %d by %d\n", xdim, ydim);
   image.Interpolate(xdim, ydim);
   xdim = image.R.Xdim;
   ydim = image.R.Ydim;

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
         // s.Px[u][v] = s.Nx[u][v] = - cos(angle1) * sin(angle2);
         // s.Py[u][v] = s.Ny[u][v] = - sin(angle1) * sin(angle2);
         // s.Pz[u][v] = s.Nz[u][v] = - cos(angle2);
         s.Px[u][v] = s.Nx[u][v] = sin(angle2) * cos(angle1);
         s.Py[u][v] = s.Ny[u][v] = sin(angle2) * sin(angle1);
         s.Pz[u][v] = s.Nz[u][v] = cos(angle2);
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
void display_surface(Surface &s, bool top_and_bottom, bool texture_map)
{
   if (texture_map)
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s.xdim, s.ydim,
         0, GL_RGB, GL_UNSIGNED_BYTE, s.texture);
      glDisable(GL_LIGHTING);
      glColor3f(1.0, 1.0, 1.0);
   }
   else
      glEnable(GL_LIGHTING);

   for (int u = 0; u < SIZE-1; u++)
      for (int v = 0; v < SIZE-1; v++)
      {
         glBegin(GL_POLYGON);
         if (texture_map) glTexCoord2f(u/(SIZE-1.0), v/(SIZE-1.0));
         else glNormal3f(s.Nx[u][v], s.Ny[u][v], s.Nz[u][v]);
         glVertex3f(s.Px[u][v], s.Py[u][v], s.Pz[u][v]);

         if (texture_map) glTexCoord2f((u+1)/(SIZE-1.0), v/(SIZE-1.0));
         else glNormal3f(s.Nx[u + 1][v], s.Ny[u + 1][v], s.Nz[u + 1][v]);
         glVertex3f(s.Px[u + 1][v], s.Py[u + 1][v], s.Pz[u + 1][v]);

         if (texture_map) glTexCoord2f((u+1)/(SIZE-1.0), (v+1)/(SIZE-1.0));
         else glNormal3f(s.Nx[u + 1][v + 1], s.Ny[u + 1][v + 1], s.Nz[u + 1][v + 1]);
         glVertex3f(s.Px[u + 1][v + 1], s.Py[u + 1][v + 1], s.Pz[u + 1][v + 1]);

         if (texture_map) glTexCoord2f(u/(SIZE-1.0), (v+1)/(SIZE-1.0));
         else glNormal3f(s.Nx[u][v + 1], s.Ny[u][v + 1], s.Nz[u][v + 1]);
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
void display_surface(Surface &s, bool top_and_bottom, bool texture_map,
   float scale, float dx, float dy, float dz)
{
   glPushMatrix();
   glTranslatef(dx, dy, dz);
   glScalef(scale, scale, scale);
   display_surface(s, top_and_bottom, texture_map);
   // display_surface(s, top_and_bottom, false);
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

   // Initialize texture maps
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   init_texture((char *)"textures/earth1.jpg", sphere.texture, sphere.xdim, sphere.ydim);
   init_texture((char *)"textures/earth0.jpg", cylinder.texture, cylinder.xdim, cylinder.ydim);
   init_texture((char *)"textures/earth1.jpg", cone.texture, cone.xdim, cone.ydim);
   init_texture((char *)"textures/earth0.jpg", donut.texture, donut.xdim, donut.ydim);
   init_texture((char *)"textures/earth1.jpg", vase.texture, vase.xdim, vase.ydim);
   init_texture((char *)"textures/earth0.jpg", mobius.texture, mobius.xdim, mobius.ydim);
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
   display_surface(sphere, false, true, 0.5, -1, 0, 0);

   init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.4, 0.6);
   display_surface(cylinder, true, true, 0.5, 1, 0, 0);

   init_material(Ka, Kd, Ks, 100 * Kp, 0.6, 0.8, 0.4);
   display_surface(cone, true, true, 0.5, 0, -1, 0);

   init_material(Ka, Kd, Ks, 100 * Kp, 0.6, 0.4, 0.8);
   display_surface(donut, false, true, 0.5, 0, 1, 0);

   init_material(Ka, Kd, Ks, 100 * Kp, 0.4, 0.8, 0.6);
   display_surface(vase, true, true, 0.5, 0, 0, -1);

   init_material(Ka, Kd, Ks, 100 * Kp, 0.4, 0.6, 0.8);
   display_surface(mobius, false, true, 0.5, 0, 0, 1);
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
   glutCreateWindow("Texture");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   glutMainLoop();
   return 0;
}

