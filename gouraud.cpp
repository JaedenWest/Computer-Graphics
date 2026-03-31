//---------------------------------------
// Program: gouraud.cpp
// Purpose: Demonstate Gouraud shading.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Program constants
#define SIZE 100
#define MOVE 0
#define LIGHT1 1
#define LIGHT2 2
#define LIGHTS 2

// Transformation variables 
int xangle = 0;
int yangle = 0;
int zangle = 0;
int mode = MOVE;

// Surface positions
float Px[SIZE][SIZE];
float Py[SIZE][SIZE];
float Pz[SIZE][SIZE];

// Surface normals
float Nx[SIZE][SIZE];
float Ny[SIZE][SIZE];
float Nz[SIZE][SIZE];

// Surface colors
float R[SIZE][SIZE];
float G[SIZE][SIZE];
float B[SIZE][SIZE];

// Light structure
struct Light
{
   float x,y,z;
   float R,G,B;
} light[LIGHTS];

// Material color
float Object_R = 1.0;
float Object_G = 1.0;
float Object_B = 1.0;

//---------------------------------------
// Initialize surface points
//---------------------------------------
void init_surface(float Xmin, float Xmax, float Ymin, float Ymax)
{
   // Initialize (x,y,z) coordinates of surface
   for (int i = 0; i < SIZE; i++)
   for (int j = 0; j < SIZE; j++)
   {
      Px[i][j] = Xmin + i * (Xmax - Xmin) / SIZE;
      Py[i][j] = Ymin + j * (Ymax - Ymin) / SIZE;
      Pz[i][j] = 0;
   }
        
   // Add randoms waves to surface
   for (int wave = 1; wave <= 10; wave++)
   {
      int rand_i = rand() % SIZE / wave;
      int rand_j = rand() % SIZE / wave;
      float length = sqrt(rand_i * rand_i + rand_j * rand_j);
      if (length >= 10)
      for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE; j++)
      {
         float angle = (rand_i * i + rand_j * j) / (length * length);
         Pz[i][j] += 0.02 * sin(angle * 2 * M_PI);
      }
   }
}

//---------------------------------------
// Initialize surface normals
//---------------------------------------
void init_normals()
{
   // Initialize surface normals
   for (int i=0; i<SIZE-1; i++)
   for (int j=0; j<SIZE-1; j++)
   {
      // Get tangents S and T
      float Sx = Px[i+1][j] - Px[i][j];
      float Sy = Py[i+1][j] - Py[i][j];
      float Sz = Pz[i+1][j] - Pz[i][j];
      float Tx = Px[i][j+1] - Px[i][j];
      float Ty = Py[i][j+1] - Py[i][j];
      float Tz = Pz[i][j+1] - Pz[i][j];
      
      // Calculate N = S cross T
      Nx[i][j] = Sy*Tz - Sz*Ty;
      Ny[i][j] = Sz*Tx - Sx*Tz;
      Nz[i][j] = Sx*Ty - Sy*Tx;

      // Make unit vector
      float length = sqrt(Nx[i][j]*Nx[i][j]
                   + Ny[i][j]*Ny[i][j]
                   + Nz[i][j]*Nz[i][j]);
      if (length > 0)
      {
         Nx[i][j] /= length;
         Ny[i][j] /= length;
         Nz[i][j] /= length;
      }
   }

   // Handle last row and column
   for (int i=0; i<SIZE; i++)
   {
      Nx[i][SIZE-1] = Nx[i][SIZE-2];
      Ny[i][SIZE-1] = Ny[i][SIZE-2];
      Nz[i][SIZE-1] = Nz[i][SIZE-2];
      Nx[SIZE-1][i] = Nx[SIZE-2][i];
      Ny[SIZE-1][i] = Ny[SIZE-2][i];
      Nz[SIZE-1][i] = Nz[SIZE-2][i];
   }
}

//---------------------------------------
// Calculate Gouraud shading of vertices
//---------------------------------------
void gouraud_shading()
{
   // Loop over all vertices
   for (int i=0; i<SIZE; i++)
   for (int j=0; j<SIZE; j++)
   {
      // Initialize vertex color
      R[i][j] = 0;
      G[i][j] = 0;
      B[i][j] = 0;

      // Loop over all lights
      for (int index=0; index<LIGHTS; index++)
      {
         // Get vector to light
         float Lx = light[index].x - Px[i][j];
         float Ly = light[index].y - Py[i][j];
         float Lz = light[index].z - Pz[i][j];
         float distance = sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
         Lx /= distance;
         Ly /= distance;
         Lz /= distance;
   
         // Calculate dot product of L and N
         float LdotN = Lx * Nx[i][j] + Ly * Ny[i][j] + Lz * Nz[i][j];
         if (LdotN < 0) LdotN = 0;

         // Calculate vertex color
         R[i][j] += LdotN * light[index].R * Object_R;
         G[i][j] += LdotN * light[index].G * Object_G;
         B[i][j] += LdotN * light[index].B * Object_B;
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
   float radius = 1.2;
   glOrtho(-radius, radius, -radius, radius, -radius, radius);
   glEnable(GL_DEPTH_TEST);

   // Initialize surface
   init_surface(-1.0, 1.0, -1.0, 1.0);
   init_normals();

   // Initialize lights
   for (int index = 0; index < LIGHTS; index++)
   {
      // Random position
      light[index].x = 0.1 + (rand() % 10) / 10.0;
      light[index].y = 0.1 + (rand() % 10) / 10.0;
      light[index].z = 0.1 + (rand() % 10) / 10.0;
       
      // Random color
      light[index].R = 0.1 + (rand() % 10) / 10.0;
      light[index].G = 0.1 + (rand() % 10) / 10.0;
      light[index].B = 0.1 + (rand() % 10) / 10.0;
      printf("Light %d, %f %f %f, %f %f %f\n", index, 
         light[index].x, light[index].y, light[index].z,
         light[index].R, light[index].G, light[index].B);
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
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Calculate Gouraud shading of vertices
   gouraud_shading();

   // Draw lines to lights
   for (int index = 0; index < LIGHTS; index++)
   {
      glBegin(GL_LINES);
      glColor3f(1,1,1);
      glVertex3f(0, 0, 0);
      glVertex3f(light[index].x, light[index].y, light[index].z);
      glEnd();
   }
    
   // Draw the surface
   int i, j;
   for (i = 0; i < SIZE-1; i++)
      for (j = 0; j < SIZE-1; j++)
      {
	 glBegin(GL_POLYGON);
	 glColor3f(R[i][j], G[i][j], B[i][j]);
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
	 glColor3f(R[i + 1][j], G[i + 1][j], B[i + 1][j]);
	 glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
	 glColor3f(R[i + 1][j + 1], G[i + 1][j + 1], B[i + 1][j + 1]);
	 glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
	 glColor3f(R[i][j + 1], G[i][j + 1], B[i][j + 1]);
	 glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
	 glEnd();
      }
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Initialize everything
   if (key == 'i')
   {
      init();
   }

   // Determine mode
   if ((key == 'm') || (key == 'M'))
   {
      printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
      mode = MOVE;
   }
   else if (key == '1')
   {
      printf("Type x y z or X Y Z to change position for  LIGHT 1.\n");
      printf("Type r g b or R G B to change color for LIGHT 1.\n");
      mode = LIGHT1;
   }
   else if (key == '2')
   {
      printf("Type x y z or X Y Z to change position for  LIGHT 2.\n");
      printf("Type r g b or R G B to change color for LIGHT 2.\n");
      mode = LIGHT2;
   }

   // Handle changes to rotation angles
   if (mode == MOVE)
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
   }

   // Handle changes to LIGHT1
   if (mode == LIGHT1)
   {
      if (key == 'x')
	 light[0].x -= 0.1;
      else if (key == 'y')
	 light[0].y -= 0.1;
      else if (key == 'z')
	 light[0].z -= 0.1;
      else if (key == 'X')
	 light[0].x += 0.1;
      else if (key == 'Y')
	 light[0].y += 0.1;
      else if (key == 'Z')
	 light[0].z += 0.1;

      else if (key == 'r')
	 light[0].R -= 0.1;
      else if (key == 'g')
	 light[0].G -= 0.1;
      else if (key == 'b')
	 light[0].B -= 0.1;
      else if (key == 'R')
	 light[0].R += 0.1;
      else if (key == 'G')
	 light[0].G += 0.1;
      else if (key == 'B')
	 light[0].B += 0.1;
   }

   // Handle changes to LIGHT2
   if (mode == LIGHT2)
   {
      if (key == 'x')
	 light[1].x -= 0.1;
      else if (key == 'y')
	 light[1].y -= 0.1;
      else if (key == 'z')
	 light[1].z -= 0.1;
      else if (key == 'X')
	 light[1].x += 0.1;
      else if (key == 'Y')
	 light[1].y += 0.1;
      else if (key == 'Z')
	 light[1].z += 0.1;

      else if (key == 'r')
	 light[1].R -= 0.1;
      else if (key == 'g')
	 light[1].G -= 0.1;
      else if (key == 'b')
	 light[1].B -= 0.1;
      else if (key == 'R')
	 light[1].R += 0.1;
      else if (key == 'G')
	 light[1].G += 0.1;
      else if (key == 'B')
	 light[1].B += 0.1;
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
   glutCreateWindow("Gouraud");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
