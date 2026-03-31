//---------------------------------------
// Program: keyboard.cpp
// Purpose: Demonstrate keyboard callbacks.
// Author:  John Gauch
// Date:    Spring 2021
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

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 0.0);
   glVertex2f(-0.5, -0.5);
   glVertex2f(-0.5, 0.5);
   glVertex2f(0.5, 0.5);
   glVertex2f(0.5, -0.5);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(0.0, 0.0, 1.0);
   glVertex2f(-0.25, -0.25);
   glVertex2f(-0.25, 0.25);
   glVertex2f(0.25, 0.25);
   glVertex2f(0.25, -0.25);
   glEnd();

   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   cout << "keyboard " << key << endl;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y)
{
   switch(key)
   {
   case GLUT_KEY_UP:
      cout << "special up\n";
      break;
   case GLUT_KEY_DOWN:
      cout << "special down\n";
      break;
   case GLUT_KEY_LEFT:
      cout << "special left\n";
      break;
   case GLUT_KEY_RIGHT:
      cout << "special right\n";
      break;
   default:
      cout << "special " << key << endl;
   }

   // Redraw objects
   glutPostRedisplay();
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
   glutCreateWindow("Keyboard");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   init();
   glutMainLoop();
   return 0;
}

