//---------------------------------------
// Program: tetris2.cpp
// Purpose: Draw seven tetris shapes.
// Author:  John Gauch
// Date:    Spring 2021
//
// TODO:    Implement rotation
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
#include <iostream>
using namespace std;

// Global variables
bool frozen = false;
const int MAX_PIECES = 100;
int xpos[MAX_PIECES];
int ypos[MAX_PIECES];
char piece[MAX_PIECES];
int cnt = 0;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-6.0, 5.0, -11.0, 10.0, -1.0, 1.0);

   // Add initial tetris pieces
   piece[0]='I'; xpos[0] = -3, ypos[0] = 9;
   piece[1]='J'; xpos[1] = -2, ypos[1] = 6;
   piece[2]='L'; xpos[2] = -1, ypos[2] = 3;
   piece[3]='O'; xpos[3] =  0, ypos[3] = 0;
   piece[4]='S'; xpos[4] =  1, ypos[4] = -3;
   piece[5]='T'; xpos[5] =  2, ypos[5] = -6;
   piece[6]='Z'; xpos[6] =  3, ypos[6] = -9;
   cnt = 7;
}

//---------------------------------------
// Draw one square
//---------------------------------------
void draw_square(float x, float y, float w, float R, float G, float B)
{
   glBegin(GL_POLYGON);
   glColor3f(R, G, B);
   glVertex2f(x-w/2, y-w/2);
   glVertex2f(x-w/2, y+w/2);
   glVertex2f(x+w/2, y+w/2);
   glVertex2f(x+w/2, y-w/2);
   glEnd();

   glLineWidth(3);
   glBegin(GL_LINE_LOOP);
   glColor3f(R/2, G/2, B/2);
   glVertex2f(x-w/2, y-w/2);
   glVertex2f(x-w/2, y+w/2);
   glVertex2f(x+w/2, y+w/2);
   glVertex2f(x+w/2, y-w/2);
   glEnd();
}

//---------------------------------------
// Draw I piece
//---------------------------------------
void draw_I(float x, float y)
{
   float R=0.65, G=0.85, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x+2,y,1, R,G,B);
}

//---------------------------------------
// Draw J piece
//---------------------------------------
void draw_J(float x, float y)
{
   float R=0, G=0, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x-1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw L piece
//---------------------------------------
void draw_L(float x, float y)
{
   float R=1, G=0.65, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw O piece
//---------------------------------------
void draw_O(float x, float y)
{
   float R=1, G=1, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw T piece
//---------------------------------------
void draw_T(float x, float y)
{
   float R=1, G=0, B=1;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
}

//---------------------------------------
// Draw S piece
//---------------------------------------
void draw_S(float x, float y)
{
   float R=0, G=1, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x-1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x+1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw Z piece
//---------------------------------------
void draw_Z(float x, float y)
{
   float R=1, G=0, B=0;
   draw_square(x,y,1, R,G,B);
   draw_square(x+1,y,1, R,G,B);
   draw_square(x,y+1,1, R,G,B);
   draw_square(x-1,y+1,1, R,G,B);
}

//---------------------------------------
// Draw the tetris board
//---------------------------------------
void draw_board()
{
   float R=0.2, G=0.2, B=0.2;
   for (int y = -10; y < 10; y++)
   for (int x = -5; x < 5; x++)
      draw_square(x,y,1, R,G,B);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw the tetris board
   draw_board();

   // Draw all of the tetris pieces
   for (int i=0; i<cnt; i++)
   {
      if (piece[i] == 'I') 
         draw_I(xpos[i], ypos[i]);
      else if (piece[i] == 'J') 
         draw_J(xpos[i], ypos[i]);
      else if (piece[i] == 'L') 
         draw_L(xpos[i], ypos[i]);
      else if (piece[i] == 'O') 
         draw_O(xpos[i], ypos[i]);
      else if (piece[i] == 'S') 
         draw_S(xpos[i], ypos[i]);
      else if (piece[i] == 'T') 
         draw_T(xpos[i], ypos[i]);
      else if (piece[i] == 'Z') 
         draw_Z(xpos[i], ypos[i]);
   }

   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Create new tetris piece
   key = toupper(key);
   if ((key == 'I') || (key == 'J') || 
       (key == 'L') || (key == 'O') || 
       (key == 'S') || (key == 'T') || 
       (key == 'Z'))
   { 
      // Add tetris piece to list
      frozen = false;
      piece[cnt] = key; 
      xpos[cnt] = 0; 
      ypos[cnt] = 9; 
      cnt++; 
   }

   // Freeze current piece
   else if (key == ' ')
      frozen = true;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y)
{
   if (!frozen)
   {
      // Move current piece
      switch(key)
      {
      case GLUT_KEY_UP:
         ypos[cnt-1]++; 
         break;
      case GLUT_KEY_DOWN:
         ypos[cnt-1]--; 
         break;
      case GLUT_KEY_LEFT:
         xpos[cnt-1]--; 
         break;
      case GLUT_KEY_RIGHT:
         xpos[cnt-1]++; 
         break;
      }

      // Redraw objects
      glutPostRedisplay();
   }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(400, 800);
   glutInitWindowPosition(200, 200);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Tetris2");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   init();
   glutMainLoop();
   return 0;
}

