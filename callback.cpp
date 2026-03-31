//---------------------------------------
// Program: callbacks.cpp
// Purpose: To demonstrate OpenGL callbacks
// Author:  John Gauch
// Date:    February 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
using namespace std;

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Global constants and variables
#define MIN_X_VIEW -50
#define MAX_X_VIEW  50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW  50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW  50

#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500

#define SLEEP_TIME 300

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize viewing window
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
           MIN_Y_VIEW, MAX_Y_VIEW, 
           MIN_Z_VIEW, MAX_Z_VIEW);

   // Turn on Z buffer
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Print parameters
   cout << "keyboard " << key << " " << x << " " << y << endl;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Print parameters
   cout << "mouse " << button << " " << state << " " 
        << x << " " << y << endl;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Print parameters
   cout << "motion " << x << " " << y << endl;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Clear screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Display object
   glFlush();
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
int counter = 0;
void timer(int value)
{
   // Print parameters
   cout << "timer " << counter++ << endl;

   // Redraw objects
   glutPostRedisplay();
   glutTimerFunc(SLEEP_TIME, timer, 0);
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
   // Print parameters
   cout << "idle " << counter++ << endl;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Initialzie display window
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Callback");
   init();

   // Initialize callback functions
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutDisplayFunc(display);
   glutTimerFunc(SLEEP_TIME, timer, 0);
   // glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
