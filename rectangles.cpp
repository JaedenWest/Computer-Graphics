//---------------------------------------
// Program: rectangles.cpp
// Purpose: Demonstrate use of mouse callbacks
//          to draw color rectangles.
// Author:  John Gauch
// Date:    Spring 2013
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
#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500
#define RECT_COUNT 1000

// Global variables 
int count = 0;
float point[RECT_COUNT][4];
float color[RECT_COUNT][3];

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

   // Clear point array
   for (int i=0; i<RECT_COUNT; i++)
   {
      for (int j=0; j<4; j++)
         point[i][j] = 0.0;
      for (int j=0; j<3; j++)
         color[i][j] = (random() % 11) * 0.1;
   }
}

//---------------------------------------
// Function to draw a rectangle
//---------------------------------------
void rectangle(float red, float green, float blue,
   float x1, float y1, float x2, float y2)
{
   // Draw rectangle
   glColor3f(red, green, blue);
   glBegin(GL_POLYGON);
   glVertex2f(x1, y1);
   glVertex2f(x2, y1);
   glVertex2f(x2, y2);
   glVertex2f(x1, y2);
   glEnd();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Adjust rectangle color
   if (count > 0)
   {
      if ((key == 'r') && (color[count-1][0] > 0.0)) color[count-1][0] -= 0.1; 
      if ((key == 'R') && (color[count-1][0] < 1.0)) color[count-1][0] += 0.1; 
      if ((key == 'g') && (color[count-1][1] > 0.0)) color[count-1][1] -= 0.1; 
      if ((key == 'G') && (color[count-1][1] < 1.0)) color[count-1][1] += 0.1; 
      if ((key == 'b') && (color[count-1][2] > 0.0)) color[count-1][2] -= 0.1; 
      if ((key == 'B') && (color[count-1][2] < 1.0)) color[count-1][2] += 0.1; 
   }

   // Load rectangle information from rectangle.txt
   if (key == 'L')
   {
      // Clear point array
      for (int i=0; i<RECT_COUNT; i++)
      {
         for (int j=0; j<4; j++)
            point[i][j] = 0.0;
         for (int j=0; j<3; j++)
            color[i][j] = 0.5;
      }

      // Read point array
      FILE *fd = fopen("rectangle.txt", "r");
      if (fscanf(fd, "%d\n", &count) != 1)
         printf("Error: could not execute fscanf command\n");
      for (int i=0; i<count; i++)
         if (fscanf(fd, "%f %f %f %f %f %f %f\n",
            &color[i][0], &color[i][1], &color[i][2],
            &point[i][0], &point[i][1], &point[i][2], &point[i][3]) != 7)
            printf("Error: could not execute fscanf command\n");
      fclose(fd);
   }

   // Save rectangle information in rectangle.txt
   if (key == 'S')
   {
      // Write point array
      FILE *fd = fopen("rectangle.txt", "w");
      fprintf(fd, "%d\n", count);
      for (int i=0; i<count; i++)
         fprintf(fd, "%f %f %f %f %f %f %f\n",
            color[i][0], color[i][1], color[i][2],
            point[i][0], point[i][1], point[i][2], point[i][3]);
      fclose(fd);
   }
   
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
   if (state == GLUT_DOWN)
   {
      point[count][0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      point[count][1] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   }

   // Handle mouse up
   else if (state == GLUT_UP) 
   {
      point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      count++; 
      glutPostRedisplay();
   }
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
   point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Clear display window
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw all rectangles
   for (int i=count; i>=0; i--)
      if ((point[i][0] != point[i][2]) || (point[i][1] != point[i][3])) 
         rectangle(color[i][0], color[i][1], color[i][2], 
            point[i][0], point[i][1], point[i][2], point[i][3]);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Rectangles");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   'r' - decrease RED by 0.1\n");
   printf("   'R' - increase RED by 0.1\n");
   printf("   'g' - decrease GREEN by 0.1\n");
   printf("   'G' - increase GREEN by 0.1\n");
   printf("   'b' - decrease BLUE by 0.1\n");
   printf("   'B' - increase BLUE by 0.1\n");
   printf("   'L' - load rectangle.txt file\n");
   printf("   'S' - save rectangle.txt file\n");
   printf("Mouse operations:\n");
   printf("   'mouse down' - start drawing rectangle\n");
   printf("   'mouse motion' - draw rubber-band rectangle\n");
   printf("   'mouse up' - finish drawing rectangle\n");
   glutMainLoop();
   return 0;
}
