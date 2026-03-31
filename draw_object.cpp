//---------------------------------------
// Program: draw_object.cpp
// Purpose: Draw object of rotation.
// Author:  John Gauch
// Date:    Spring 2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global constants
const int MIN_X_VIEW = -1;
const int MAX_X_VIEW = 1;
const int MIN_Y_VIEW = -1;
const int MAX_Y_VIEW = 1;
const int MIN_Z_VIEW = -1;
const int MAX_Z_VIEW = 1;
const int MIN_X_SCREEN = 0;
const int MAX_X_SCREEN = 640;
const int MIN_Y_SCREEN = 0;
const int MAX_Y_SCREEN = 480;

// Curve model
const int POINTS = 1000;
float Mx[POINTS];
float My[POINTS];
int count = 0;

// Surface model
const int SIZE = 50;
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];

// Display variables
int Xangle = 5;
int Yangle = 5;
int Zangle = 5;
int mouse_state = GLUT_UP;

//---------------------------------------
// Define object of revolution
//---------------------------------------
void define_object()
{
   // Create object profile
   float scale = (float)(count-1) / (float)(SIZE);
   for (int i = 0; i <= SIZE; i++)
   {
      Px[i][0] = Mx[(int)(i*scale)];
      Py[i][0] = My[(int)(i*scale)];
      Pz[i][0] = 0.0;
   }

   // Perform rotation around X axis
   for (int j = 1; j <= SIZE; j++)
   {
      float theta = j * 2 * M_PI / SIZE;
      float cos_theta = cos(theta);
      float sin_theta = sin(theta);
      for (int i = 0; i <= SIZE; i++)
      {
	 Px[i][j] = Px[i][0];
	 Py[i][j] = Py[i][0] * cos_theta - Pz[i][0] * sin_theta;
	 Pz[i][j] = Py[i][0] * sin_theta + Pz[i][0] * cos_theta;
      }
   }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);

   // Create initial profile
   count = SIZE;
   for (int i=0; i<count; i++)
   {
      float theta = i * 2 * M_PI / SIZE;
      Mx[i] = (float) i / (float) SIZE - 0.5;
      My[i] = 0.02 * cos(10*theta) + 0.2 * sin(theta) + 0.5;
   }
   define_object();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Draw curve
   if (mouse_state == GLUT_DOWN)
   {
      glColor3f(0.0, 1.0, 0.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i < count; i++)
         glVertex2f(Mx[i], My[i]);
      glEnd();
   }

   // Draw surface
   if (mouse_state == GLUT_UP)
   {
      glColor3f(1.0, 1.0, 0.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef(Xangle, 1.0, 0.0, 0.0);
      glRotatef(Yangle, 0.0, 1.0, 0.0);
      glRotatef(Zangle, 0.0, 0.0, 1.0);
   
      // Draw object surface
      for (int i = 0; i <= SIZE; i++)
      {
         glBegin(GL_LINE_STRIP);
         for (int j = 0; j <= SIZE; j++)
	    glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
         glEnd();
      }
      for (int j = 0; j <= SIZE; j++)
      {
         glBegin(GL_LINE_STRIP);
         for (int i = 0; i <= SIZE; i++)
	    glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
         glEnd();
      }
   }

   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update rotation angles
   if (key == 'x') Xangle -= 5;
   if (key == 'X') Xangle += 5;
   if (key == 'y') Yangle -= 5;
   if (key == 'Y') Yangle += 5;
   if (key == 'z') Zangle -= 5;
   if (key == 'Z') Zangle += 5;
   if (key == 'q') exit(0);

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse down
   mouse_state = state;
   if (state == GLUT_DOWN)
   {
      count = 0;
      Mx[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      My[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      if (count+1 < POINTS) count++; 
   }

   // Handle mouse up
   else if (state == GLUT_UP) 
   {
      Mx[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      My[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      if (count+1 < POINTS) count++; 
      define_object();
   }

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   Mx[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   My[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   if (count+1 < POINTS) count++; 

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_X_SCREEN, MAX_Y_SCREEN);
   glutInitWindowPosition(MAX_X_SCREEN/2, MAX_Y_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Draw Object");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   glutMainLoop();
   return 0;
}
