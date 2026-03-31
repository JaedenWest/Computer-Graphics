//---------------------------------------
// Program: painter.cpp
// Purpose: Demonstrate painter's algorithm.
// Author:  John Gauch
// Date:    Spring 2020
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Scene.h"
#include <iostream>
using namespace std;

// Global variables
Scene scene;
char axis = ' ';
bool painters = true;

//---------------------------------------
// Get color on rainbow scale
//---------------------------------------
void get_color(int angle, float &r, float &g, float &b)
{
   // Interpolate between R and G
   if ((angle >= 0) && (angle < 120))
   {
      float fraction = angle / 120.0;
      r = 1.0 - fraction;
      g = fraction;
      b = 0.0;
   }

   // Interpolate between G and B
   else if ((angle >= 120) && (angle < 240))
   {
      float fraction = (angle - 120) / 120.0;
      r = 0.0;
      g = 1.0 - fraction;
      b = fraction;
   }

   // Interpolate between B and R
   else if ((angle >= 240) && (angle < 360))
   {
      float fraction = (angle - 240) / 120.0;
      r = fraction;
      g = 0.0;
      b = 1.0 - fraction;
   }

   // Normalize length
   float length = sqrt(r * r + g * g + b * b);
   if (length > 0) r /= length;
   if (length > 0) g /= length;
   if (length > 0) b /= length;
}

//---------------------------------------
// Create rainbow colored donut
//---------------------------------------
void donut(int rows, int cols, Scene & scene)
{
    // Surface mesh
    float Px[rows + 1][cols + 1];
    float Py[rows + 1][cols + 1];
    float Pz[rows + 1][cols + 1];
    
    // Create X-Y profile
    for (int r = 0; r <= rows; r++)
    {
        float theta = r * 2 * M_PI / rows;
        Px[r][0] = 0.2 * cos(theta) + 0.5;
        Py[r][0] = 0.2 * sin(theta);
        Pz[r][0] = 0.0;
    }
    
    // Perform rotation around Y axis
    for (int c = 1; c <= cols; c++)
    {
        float theta = c * 2 * M_PI / cols;
        float cos_theta = cos(theta);
        float sin_theta = sin(theta);
        for (int r = 0; r <= rows; r++)
        {
            Px[r][c] = Px[r][0] * cos_theta - Pz[r][0] * sin_theta;
            Py[r][c] = Py[r][0];
            Pz[r][c] = Px[r][0] * sin_theta + Pz[r][0] * cos_theta;
        }
    }
    
    // Store polygons in scene
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
        {
            Vertex v;
            float angle = c * 360.0 / cols;
            get_color(angle, v.R, v.G, v.B);

            Polygon p;
            v.Px = Px[r][c];
            v.Py = Py[r][c];
            v.Pz = Pz[r][c];
            p.vertex.push_back(v);
            
            v.Px = Px[r+1][c];
            v.Py = Py[r+1][c];
            v.Pz = Pz[r+1][c];
            p.vertex.push_back(v);
            
            v.Px = Px[r+1][c+1];
            v.Py = Py[r+1][c+1];
            v.Pz = Pz[r+1][c+1];
            p.vertex.push_back(v);
            
            v.Px = Px[r][c+1];
            v.Py = Py[r][c+1];
            v.Pz = Pz[r][c+1];
            p.vertex.push_back(v);
            scene.polygon.push_back(p);
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
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Initialize scene
   donut(60,60,scene);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Quit program
   if (key == 'q')
      exit(0);

   // Save rotation axis
   if (key == 'x')
      axis = key;
   if (key == 'y')
      axis = key;
   if (key == 'z')
      axis = key;

   // Turn painters on/off
   if (key == 'p')
   {
      painters = !painters;
      cout << "painters = " << painters << endl;
   }
   
   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);

   // Rotate scene 
   Matrix m;
   m.identity();
   if (axis == 'x')
      m.rotateX(3);
   if (axis == 'y')
      m.rotateY(3);
   if (axis == 'z')
      m.rotateZ(3);
   scene.transform(m);

   // Apply painter's algorithm to display scene
   if (painters)
   {
      scene.depth();
      scene.sort();
   }
   scene.display();
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
   glutCreateWindow("Painter");
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   init();
   cout << "Commands for painters algorithm program:\n";
   cout << "  x - rotate around x axis\n";
   cout << "  y - rotate around y axis\n";
   cout << "  z - rotate around z axis\n";
   cout << "  p - turn painters on/off\n";
   glutMainLoop();
   return 0;
}
