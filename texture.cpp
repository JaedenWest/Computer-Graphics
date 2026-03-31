//---------------------------------------
// Program: texture.cpp
// Purpose: Texture map Mandelbrot set
//          image onto a cube model.
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
#define ROTATE 1
#define TRANSLATE 2
int xangle = 10;
int yangle = 10;
int zangle = 10;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;

// Texture variables
#define XDIM 256
#define YDIM 256
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
   float max_magnitude = 1e30;

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
   int i;
   unsigned char r_map[256] = {0};
   unsigned char g_map[256] = {0};
   unsigned char b_map[256] = {0};
   for (i = 0; i < 128; i++)
   {
      b_map[i] = 255 - 2 * i;
      g_map[i] = 2 * i;
      g_map[i+128] = 255 - 2 * i;
      r_map[i+128] = 2 * i;
   }
   r_map[0] = g_map[0] = b_map[0] = 0;

   int x, y;
   for (y = 0; y < YDIM; y++)
      for (x = 0; x < XDIM; x++)
      {
         float C_re = center_re - radius + 2 * radius * x / XDIM;
         float C_im = center_im - radius + 2 * radius * y / YDIM;
         int value = 2 * mandelbrot(C_re, C_im, 128);
         image[y][x][0] = r_map[value];
         image[y][x][1] = g_map[value];
         image[y][x][2] = b_map[value];
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
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(ex, ey, ez);
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
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(cx, cy, cz);
   glEnd();
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);

   // Init texture
   init_mandelbrot();
   glEnable(GL_TEXTURE_2D);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, XDIM, YDIM, 0, 
      GL_RGB, GL_UNSIGNED_BYTE, image);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Draw objects
   block(-1, -1, -1, 1, 1, 1);
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

   // Handle TRANSLATE
   if (mode == TRANSLATE)
   {
      if (key == 'x')
	 xpos -= 5;
      else if (key == 'y')
	 ypos -= 5;
      else if (key == 'z')
	 zpos -= 5;
      else if (key == 'X')
	 xpos += 5;
      else if (key == 'Y')
	 ypos += 5;
      else if (key == 'Z')
	 zpos += 5;
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
      xangle += (y - ydown);
      yangle -= (x - xdown);
      zangle = 0;
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if ((mode == TRANSLATE) && (state == GLUT_UP))
   {
      xpos += (x - xdown);
      ypos -= (y - ydown);
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
   glutCreateWindow("Texture");
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
    printf("Mouse operations:\n");
    printf("   'mouse down' - start selecting rotation direction\n");
    printf("   'mouse up' - finish selecting rotation direction\n");
   glutMainLoop();
   return 0;
}
