//---------------------------------------
// Program: Homework4.cpp
// Purpose: texture mapping
// Author:  Jaeden West
// Date:    3/26/2026
//---------------------------------------
#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"

// Global variables
#define ROTATE 1
#define TRANSLATE 2
int mode = ROTATE;

int xangle = 10, yangle = 10, zangle = 10;
int xpos = 0, ypos = 0, zpos = 0;
float dt = 0.01f;

class Block
{
   // texture data
   unsigned char *texture; // 1D array of RGB values
   int xdim, ydim;

   // cube dimensions
   float size;
   float xmin, xmax, ymin, ymax, zmin, zmax;

   // orbit
   float angle;
   float orbitRadius;
   float orbitSpeed;

public:
   // object transformaition
   float Px, Py, Pz;
   float Ax, Ay, Az;
   // velocity
   float Vx, Vy, Vz;

   // constructor
   Block(const char *filename, float px = 0, float py = 0, float pz = 0, float size = 0.5f)
   {
      Px = px;
      Py = py;
      Pz = pz;
      this->size = size;
      Ax = 0;
      Ay = 0;
      Az = 0;
      Vx = 0.1f;
      Vy = 0.0f;
      Vz = 0.0f;

      xmin = -size;
      xmax = size;
      ymin = -size;
      ymax = size;
      zmin = -size;
      zmax = size;

      angle = 0.0f;
      // spread cubes out in rings
      orbitRadius = 1.0f + (rand() % 100) / 100.0f * 2.0f;

      // different speeds
      orbitSpeed = 0.1f + (rand() % 100) / 100.0f;

      init_texture(filename);
   }
   // intialize texture
   void init_texture(const char *filename)
   {
      // Read jpg image
      im_color image;
      image.ReadJpg(filename);
      printf("Reading image %s\n", filename);
      xdim = 1;
      while (xdim < image.R.Xdim)
         xdim *= 2;
      xdim /= 2;
      ydim = 1;
      while (ydim < image.R.Ydim)
         ydim *= 2;
      ydim /= 2;
      image.Interpolate(xdim, ydim);
      printf("Interpolating %dx%d to %dx%d\n",
             image.R.Xdim, image.R.Ydim, xdim, ydim);

      // Copy image into texture array
      texture = (unsigned char *)malloc((unsigned int)(xdim * ydim * 3));
      int index = 0;
      for (int y = 0; y < ydim; y++)
         for (int x = 0; x < xdim; x++)
         {
            texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
         }
   }
   void draw()
   {
      glPushMatrix();

      // Apply translation & rotation
      glTranslatef(Px, Py, Pz);
      glRotatef(Ax, 1, 0, 0);
      glRotatef(Ay, 0, 1, 0);
      glRotatef(Az, 0, 0, 1);

      // Bind texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

      // Draw cube
      block(xmin, ymin, zmin, xmax, ymax, zmax);

      glPopMatrix();
   }

   void update()
   {
      // orbit
      angle += orbitSpeed * dt;
      Px = orbitRadius * cos(angle);
      Pz = orbitRadius * sin(angle);

      Ax += 50 * dt;
      Ay += 30 * dt;
      Az += 10 * dt;

      // bouncing off edges
      if (Px + size >= 3.0f || Px - size <= -3.0f)
      {
         Vx *= -1;
      }
      if (Py + size >= 3.0f || Py - size <= -3.0f)
      {
         Vy *= -1;
      }
   }

private:
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
      glBegin(GL_POLYGON); // Max texture coord 1.0
      glTexCoord2f(0.0, 0.0);
      glVertex3f(ax, ay, az);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(bx, by, bz);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(cx, cy, cz);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(dx, dy, dz);
      glEnd();

      glBegin(GL_POLYGON); // Max texture coord 1.0
      glTexCoord2f(0.0, 0.0);
      glVertex3f(ex, ey, ez);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(ax, ay, az);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(dx, dy, dz);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(hx, hy, hz);
      glEnd();

      glBegin(GL_POLYGON); // Max texture coord 1.0
      glTexCoord2f(0.0, 0.0);
      glVertex3f(fx, fy, fz);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(ex, ey, ez);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(hx, hy, hz);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(gx, gy, gz);
      glEnd();

      glBegin(GL_POLYGON); // Max texture coord 1.0
      glTexCoord2f(0.0, 0.0);
      glVertex3f(bx, by, bz);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(fx, fy, fz);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(gx, gy, gz);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(cx, cy, cz);
      glEnd();

      glBegin(GL_POLYGON); // Max texture coord 3.0
      glTexCoord2f(0.0, 0.0);
      glVertex3f(ax, ay, az);
      glTexCoord2f(1.0, 0.0);
      glVertex3f(ex, ey, ez);
      glTexCoord2f(1.0, 1.0);
      glVertex3f(fx, fy, fz);
      glTexCoord2f(0.0, 1.0);
      glVertex3f(bx, by, bz);
      glEnd();

      glBegin(GL_POLYGON); // Max texture coord 3.0
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
};

class Sun
{
public:
   float radius;
   unsigned char *texture; // 1D array of RGB values
   int xdim, ydim;

   Sun(const char *filename, float r)
   {
      radius = r;
      init_texture(filename);
   }

   // intialize texture
   void init_texture(const char *filename)
   {
      // Read jpg image
      im_color image;
      image.ReadJpg(filename);
      printf("Reading image %s\n", filename);
      xdim = 1;
      while (xdim < image.R.Xdim)
         xdim *= 2;
      xdim /= 2;
      ydim = 1;
      while (ydim < image.R.Ydim)
         ydim *= 2;
      ydim /= 2;
      image.Interpolate(xdim, ydim);
      printf("Interpolating %dx%d to %dx%d\n",
             image.R.Xdim, image.R.Ydim, xdim, ydim);

      // Copy image into texture array
      texture = (unsigned char *)malloc((unsigned int)(xdim * ydim * 3));
      int index = 0;
      for (int y = 0; y < ydim; y++)
         for (int x = 0; x < xdim; x++)
         {
            texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
         }
   }

   void draw()
   {
      glPushMatrix();

      // Bind texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

      // Draw sphere
      GLUquadric *quad = gluNewQuadric();
      gluQuadricTexture(quad, GL_TRUE);
      gluSphere(quad, radius, 36, 18);
      gluDeleteQuadric(quad);

      glPopMatrix();
   }
};

//  vector of class blocks objects
std::vector<Block>
    blocks;
// create sun
Sun sun("textures/sun.jpg", 0.5f);

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-3.0, 3.0, -3.0, 3.0, -3.0, 3.0);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

   // Create some blocks
   blocks.push_back(Block("textures/rock3.jpg", -2.0, 1.0, -1.0, 0.5));
   blocks.push_back(Block("textures/minecraftStone.jpg", 2.0, 0.5, 1.0, 0.3));
   blocks.push_back(Block("textures/rock2.jpg", -1.5, 2.0, 1.0, 0.4));
   blocks.push_back(Block("textures/rock1.jpg", 1.0, 2.5, -0.5, 0.1));
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

   glPushMatrix();

   sun.draw();
   for (size_t i = 0; i < blocks.size(); i++)
   {
      blocks[i].draw();
   }
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

void idle()
{
   for (size_t i = 0; i < blocks.size(); i++)
   {

      blocks[i].update();
      // printf("%f %f %f\n", blocks[i].Px, blocks[i].Py, blocks[i].Pz);
      // printf("%f %f %f\n", blocks[i].Ax, blocks[i].Ay, blocks[i].Az);
      // printf("%f %f %f\n", blocks[i].Vx, blocks[i].Vy, blocks[i].Vz);
   }
   glutPostRedisplay();
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
   glutIdleFunc(idle);
   glutCreateWindow("Texture");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   printf("Keyboard commands:\n");
   printf("   't' or 'T' - go to translate mode\n");
   printf("   'r' or 'R' - go to rotate mode\n");
   printf("   'x' - rotate or translate on x-axis -5\n");
   printf("   'X' - rotate or translate on x-axis +5\n");
   printf("   'y' - rotate or translate on y-axis -5\n");
   printf("   'Y' - rotate or translate on y-axis +5\n");
   glutMainLoop();
   return 0;
}
