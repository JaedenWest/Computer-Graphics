//---------------------------------------
// Program: city.cpp
// Purpose: Draw city with 2D roads and buildings.
// Author:  John Gauch
// Date:    Spring 2025
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
using namespace std;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.1, 0.3, 0.1, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 15.0, 0.0, 15.0, 0.0, 15.0);
}

//---------------------------------------
// Draw 2D building with dark outline
//---------------------------------------
void draw_building(float x1, float y1, float x2, float y2,
   float R, float G, float B)
{
   // Draw polygon
   glBegin(GL_POLYGON);
   glColor3f(R, G, B);
   glVertex2f(x1, y1);
   glVertex2f(x1, y2);
   glVertex2f(x2, y2);
   glVertex2f(x2, y1);
   glEnd();
}

//---------------------------------------
// Draw 2D sidewalk 
//---------------------------------------
void draw_sidewalk(float x1, float y1, float x2, float y2)
{
   // Get sidewalk direction
   float width = 0.7;
   float dx = y1-y2;
   float dy = x2-x1;
   float len = sqrt(dx*dx+dy*dy);

   // Draw grey sidewalk
   dx = 0.5 * width * dx / len;
   dy = 0.5 * width * dy / len;
   glBegin(GL_POLYGON);
   glColor3f(0.8, 0.8, 0.8);
   glVertex2f(x1-dx, y1-dy);
   glVertex2f(x1+dx, y1+dy);
   glVertex2f(x2+dx, y2+dy);
   glVertex2f(x2-dx, y2-dy);
   glEnd();
}

//---------------------------------------
// Draw 2D road 
//---------------------------------------
void draw_road(float x1, float y1, float x2, float y2)
{
   // Get road direction
   float width = 0.5;
   float dx = y1-y2;
   float dy = x2-x1;
   float len = sqrt(dx*dx+dy*dy);
   dx = 0.5 * width * dx / len;
   dy = 0.5 * width * dy / len;

   // Draw black road
   glBegin(GL_POLYGON);
   glColor3f(0.2, 0.2, 0.2);
   glVertex2f(x1-dx, y1-dy);
   glVertex2f(x1+dx, y1+dy);
   glVertex2f(x2+dx, y2+dy);
   glVertex2f(x2-dx, y2-dy);
   glEnd();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   draw_building(1,2, 5,4, 0.8,0.2,0.2);
   draw_building(7,1, 9,4, 0.4,0.5,0.2);
   draw_building(11,1, 14,4, 0.5,0.2,0.5);
   draw_building(3,6, 5,7, 0.6,0.3,0.2);
   draw_building(7,6, 8,8, 0.1,0.3,0.7);
   draw_building(10,6, 12,9, 0.3,0.1,0.3);
   draw_building(3,9, 5,11, 0.7,0.2,0.3);
   draw_building(7,10, 8,13, 0.5,0.3,0.1);
   draw_building(10,10, 12,12, 0.1,0.3,0.8);

   draw_sidewalk(0,5, 15,5);
   draw_sidewalk(13,5, 13,15);
   draw_sidewalk(6,2, 6,13);

   draw_road(0,5, 15,5);
   draw_road(13,5, 13,15);
   draw_road(6,2, 6,13);

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
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("City");
   glutDisplayFunc(display);
   init();
   glutMainLoop();
   return 0;
}
