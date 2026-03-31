//---------------------------------------
// Program: whack_a_mole.cpp
// Purpose: Demonstrate use of idle callbacks
//          to implement whack a mole game.
// Author:  John Gauch
// Date:    February 2014
//---------------------------------------
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "timer.h"
using namespace std;
Timer game_time;
Timer mole_time;

// Global variables
#define SIZE 500
#define RADIUS 0.1
int score = 0;
int board[3][3];

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   srandom(time(NULL));

   // Initialize board
   score = 0;
   for (int x=0; x<3; x++)
   for (int y=0; y<3; y++)
      board[y][x] = 0;
   board[random()%3][random()%3] = 1;

   // Start timers
   game_time.start_timer();
   mole_time.start_timer();
}

//---------------------------------------
// Draw solid color elipse
//---------------------------------------
void draw_elipse(float cx, float cy, float rx, float ry)
{
   // Generate vertices on elipse
   glBegin(GL_POLYGON);
   for (int angle = 0; angle < 360; angle++)
   {
      // Specify vertex
      float radians = M_PI * angle / 180.0;
      float x = cx + rx * cos(radians);
      float y = cy + ry * sin(radians);
      glVertex2f(x, y);
   }
   glEnd();
}

//---------------------------------------
// Draw solid color circle
//---------------------------------------
void draw_circle(float cx, float cy, float radius)
{
   // Generate vertices on circle
   glBegin(GL_POLYGON);
   for (int angle = 0; angle < 360; angle++)
   {
      // Specify vertex
      float radians = M_PI * angle / 180.0;
      float x = cx + radius * cos(radians);
      float y = cy + radius * sin(radians);
      glVertex2f(x, y);
   }
   glEnd();
}

//---------------------------------------
// Draw face
//---------------------------------------
void draw_face(float cx, float cy, float radius)
{
   float r2 = radius/2;
   float r3 = radius/3;
   float r4 = radius/4;
   float r5 = radius/5;
   float r6 = radius/6;
   float r9 = radius/9;

   // Draw face
   glColor3f(0.9, 0.5, 0.3);
   draw_circle(cx, cy, radius);

   // Draw eyes
   glColor3f(1.0, 0.9, 0.9);
   draw_elipse(cx-r3, cy+r4, r4, r5);
   draw_elipse(cx+r3, cy+r4, r4, r5);

   glColor3f(0.2, 0.2, 0.2);
   draw_circle(cx-r3, cy+r4, r6);
   draw_circle(cx+r3, cy+r4, r6);

   // Draw nose and mouth
   glColor3f(0.2, 0.2, 0.2);
   draw_circle(cx, cy-r4, r5); 
   draw_elipse(cx, cy-r2, r3, r9); 
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
   // End game
   game_time.end_timer();
   if (game_time.elapsed_time() > 20)
   {
      printf("G A M E   O V E R ! ! !\n");
      exit(0);
   }

   // Update board
   mole_time.end_timer();
   if (mole_time.elapsed_time() > 1.5)
   {
      for (int x=0; x<3; x++)
      for (int y=0; y<3; y++)
         board[y][x] = 0;
      board[random()%3][random()%3] = 1;
      mole_time.start_timer();
      glutPostRedisplay();
   }
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Handle mouse down
   if (state == GLUT_DOWN)
   {
      // Calculate float coordinates of mouse
      float fx = - 2.0 * (SIZE-x) / SIZE + 1.0;
      float fy =   2.0 * (SIZE-y) / SIZE - 1.0;

      // Check all mole locations
      for (int x=0; x<3; x++)
      for (int y=0; y<3; y++)
      if (board[y][x] == 1)
      {
         // Calculate distance to mole
         float cx = (x-1)*6*RADIUS;
         float cy = (y-1)*6*RADIUS;
         float dx = fx-cx;
         float dy = fy-cy;
         float distance = sqrt(dx*dx+dy*dy);

         // Check if mole was hit
         if (distance < RADIUS)
         {
            // Update score
            game_time.end_timer();
            printf("score: %d, time: %1.2f sec\n", ++score, game_time.elapsed_cpu());

            // Update board
            for (int x=0; x<3; x++)
            for (int y=0; y<3; y++)
               board[y][x] = 0;
            board[random()%3][random()%3] = 1;
            mole_time.start_timer();
            glutPostRedisplay();
         }
      }
   }
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Draw green background
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.1, 0.7, 0.1);
   glBegin(GL_POLYGON);
   glVertex2f(-1.0, -1.0);
   glVertex2f( 1.0, -1.0);
   glVertex2f( 1.0,  1.0);
   glVertex2f(-1.0,  1.0);
   glEnd();

   // Draw holes and faces
   for (int x=0; x<3; x++)
   for (int y=0; y<3; y++)
   {
      // Draw hole
      glColor3f(0.2, 0.2, 0.2);
      float cx = (x-1)*6*RADIUS;
      float cy = (y-1)*6*RADIUS;
      draw_elipse(cx, cy, 2*RADIUS, 1.5*RADIUS);

      // Draw face
      if (board[y][x] == 1)
         draw_face( cx, cy, RADIUS);
   }
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(SIZE, SIZE);
   glutInitWindowPosition(SIZE/2, SIZE/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Whack-A-Mole");
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}

