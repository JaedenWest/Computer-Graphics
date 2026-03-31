//---------------------------------------
// Program: balls4.cpp
// Purpose: Use Phong shading to display
//          balls attracted towards each other.
// Author:  John Gauch
// Date:    April 2014   
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

// Constants
#define WINDOW 600
#define RADIUS 100
#define COUNT 30
#define SPEED 0.2
#define SIZE 20
#define STEP 0.2
#define SLEEP_TIME 20

// Ball variables
float Px[COUNT];
float Py[COUNT];
float Pz[COUNT];
float Vx[COUNT];
float Vy[COUNT];
float Vz[COUNT];
float Radius[COUNT];
float Mass[COUNT];
float Red[COUNT];
float Green[COUNT];
float Blue[COUNT];
float X[SIZE+1][SIZE+1];
float Y[SIZE+1][SIZE+1];
float Z[SIZE+1][SIZE+1];
float Bounce = -1;

// Flocking variables
float Fx = 0;
float Fy = 0;

#include "shading.cpp"

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Initialize balls 
//---------------------------------------
void init_balls()
{
   // Initialize balls with random positions and colors
   int i, j;
   for (i=0; i<COUNT; i++)
   {
      // Calculate random positions
      Px[i] = myrand(-RADIUS, RADIUS);
      Py[i] = myrand(-RADIUS, RADIUS);
      Pz[i] = 0;
      Vx[i] = 0;
      Vy[i] = 0;
      Vz[i] = 0;
      Radius[i] = myrand(0.03 * RADIUS, 0.06 * RADIUS);
      Mass[i] = (4.0 / 3.0) * M_PI * Radius[i] * Radius[i] * Radius[i];

      // Genterate random colors
      Red[i] = myrand(0, 1);
      Green[i] = myrand(0, 1);
      Blue[i] = myrand(0, 1);
   } 

   // Initialize ball surface
   for (i = 0; i <= SIZE; i++)
   {
      float angle1 = 2 * i * M_PI / SIZE;
      for (j = 0; j <= SIZE; j++)
      {
         float angle2 = j * M_PI / SIZE;
         X[i][j] = cos(angle1) * sin(angle2);
         Y[i][j] = sin(angle1) * sin(angle2);
         Z[i][j] = cos(angle2);
      }
   }
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
   glOrtho(-RADIUS, RADIUS, -RADIUS, RADIUS, -RADIUS, RADIUS);
   glEnable(GL_DEPTH_TEST);

   // Initialize smooth shading
   glShadeModel(GL_SMOOTH);
   init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
   init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1);
   init_light(GL_LIGHT2, 1, 1, 0, 1, 1, 1);

   // Initialize bouncing balls
   init_balls();
}

//---------------------------------------
// Draw 3D ball 
//---------------------------------------
void draw_ball(float centerX, float centerY, 
float centerZ, float radius)
{
   // Draw the surface
   int i, j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
      {
         glBegin(GL_POLYGON);
         glNormal3f(X[i][j], Y[i][j], Z[i][j]);
         glVertex3f(centerX+radius*X[i][j], 
            centerY+radius*Y[i][j], centerZ+radius*Z[i][j]);

         glNormal3f(X[i][j + 1], Y[i][j + 1], Z[i][j + 1]);
         glVertex3f(centerX+radius*X[i][j + 1], 
            centerY+radius*Y[i][j + 1], centerZ+radius*Z[i][j + 1]);

         glNormal3f(X[i + 1][j + 1], Y[i + 1][j + 1], Z[i + 1][j + 1]);
         glVertex3f(centerX+radius*X[i + 1][j + 1], 
            centerY+radius*Y[i + 1][j + 1], centerZ+radius*Z[i + 1][j + 1]);

         glNormal3f(X[i + 1][j], Y[i + 1][j], Z[i + 1][j]);
         glVertex3f(centerX+radius*X[i + 1][j], 
            centerY+radius*Y[i + 1][j], centerZ+radius*Z[i + 1][j]);
         glEnd();
      }
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

   // Draw the bouncing balls
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Initialize material properties
      init_material(Ka, Kd, Ks, 100 * Kp, Red[i], Green[i], Blue[i]);

      // Draw ball at current location
      draw_ball(Px[i], Py[i], Pz[i], Radius[i]);
   }
   glutSwapBuffers();
}

//---------------------------------------
// Move ball towards the leader
//---------------------------------------
void move_ball(int leader, int ball)
{
   // Calculate distance to target
   float dx = Px[ball] - Fx;
   float dy = Py[ball] - Fy;

   // Take step towards target
   float distance = sqrt(dx * dx + dy * dy);
   float sum_radius = Radius[leader] + Radius[ball];
   if (distance - sum_radius > STEP)
   {
      Px[ball] -= (STEP * dx) / distance; 
      Py[ball] -= (STEP * dy) / distance; 
   }
}

//---------------------------------------
// Bounce two balls off of each other
//---------------------------------------
void bounce_balls(int ball1, int ball2)
{
   // Error checking
   if (ball1 == ball2) 
      return;

   // Check if balls collide 
   float dx = Px[ball1] - Px[ball2];
   float dy = Py[ball1] - Py[ball2];
   float distance = sqrt(dx * dx + dy * dy);
   float sum_radius = Radius[ball1] + Radius[ball2];
   float overlap = sum_radius - distance;
   if (overlap < 0)
      return;

   // Move balls so they do not touch
   Px[ball1] += 0.5 * overlap * dx / distance;
   Py[ball1] += 0.5 * overlap * dy / distance;
   Px[ball2] -= 0.5 * overlap * dx / distance;
   Py[ball2] -= 0.5 * overlap * dy / distance;

   // Get speeds of each ball before collision
   float speed1 = sqrt(Vx[ball1] * Vx[ball1] + Vy[ball1] * Vy[ball1]);
   float speed2 = sqrt(Vx[ball2] * Vx[ball2] + Vy[ball2] * Vy[ball2]);

   // Get angles for each ball before collision
   float angle1 = atan2(Vy[ball1], Vx[ball1]);
   float angle2 = atan2(Vy[ball2], Vx[ball2]);

   // Get collision angle
   float collisionAngle = atan2(dy, dx);

   // Rotate velocity vectors 
   float rotatedVelocityX1 = speed1 * cos(angle1 - collisionAngle);
   float rotatedVelocityY1 = speed1 * sin(angle1 - collisionAngle);
   float rotatedVelocityX2 = speed2 * cos(angle2 - collisionAngle);
   float rotatedVelocityY2 = speed2 * sin(angle2 - collisionAngle);

   // Calculate velocities using conservation of momentum
   float finalVelocityX1 = (
      (Mass[ball1] - Mass[ball2]) * rotatedVelocityX1 + 
      (Mass[ball2] + Mass[ball2]) * rotatedVelocityX2) / 
      (Mass[ball1] + Mass[ball2]);
   float finalVelocityX2 = (
      (Mass[ball1] + Mass[ball1]) * rotatedVelocityX1 + 
      (Mass[ball2] - Mass[ball1]) * rotatedVelocityX2) / 
      (Mass[ball1] + Mass[ball2]);
   float finalVelocityY1 = rotatedVelocityY1;
   float finalVelocityY2 = rotatedVelocityY2;

   // Rotate angles back again so the collision angle is preserved
   Vx[ball1] = cos(collisionAngle) * finalVelocityX1 + 
               cos(collisionAngle + M_PI/2) * finalVelocityY1;
   Vy[ball1] = sin(collisionAngle) * finalVelocityX1 + 
               sin(collisionAngle + M_PI/2) * finalVelocityY1;
   Vx[ball2] = cos(collisionAngle) * finalVelocityX2 + 
               cos(collisionAngle + M_PI/2) * finalVelocityY2;
   Vy[ball2] = sin(collisionAngle) * finalVelocityX2 + 
               sin(collisionAngle + M_PI/2) * finalVelocityY2;
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
   // Update target position
   if (rand() % 100 == 0)
   {
      Fx = myrand(-RADIUS, RADIUS);
      Fy = myrand(-RADIUS, RADIUS);
   }

   // Update position and velocity of balls
   for (int i = 0; i < COUNT; i++)
   {
      // Move ball towards target
      move_ball(0, i);
         
      // Bounce balls off each other
      for (int j = 0; j < COUNT; j++)
         bounce_balls(i, j);

      // Bounce balls off walls
      if (Px[i] > RADIUS - Radius[i]) 
         {Px[i] = RADIUS - Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] > RADIUS - Radius[i]) 
         {Py[i] = RADIUS - Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] > RADIUS - Radius[i]) 
         {Pz[i] = RADIUS - Radius[i]; Vz[i] *= Bounce; }
      if (Px[i] < -RADIUS + Radius[i]) 
         {Px[i] = -RADIUS + Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] < -RADIUS + Radius[i]) 
         {Py[i] = -RADIUS + Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] < -RADIUS + Radius[i]) 
         {Pz[i] = -RADIUS + Radius[i]; Vz[i] *= Bounce; }

      // Update ball position
      Px[i] += Vx[i];
      Py[i] += Vy[i];
      Pz[i] += Vz[i];
   }
   glutPostRedisplay();
   glutTimerFunc(SLEEP_TIME, timer, 0);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(WINDOW, WINDOW);
   glutInitWindowPosition(WINDOW/2, WINDOW/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Balls");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutTimerFunc(SLEEP_TIME, timer, 0);
   glutMainLoop();
   return 0;
}
