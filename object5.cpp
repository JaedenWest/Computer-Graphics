//---------------------------------------
// Program: object5.cpp
// Purpose: Use vertex table to display
//          objects of rotation.
// Author:  John Gauch
// Date:    February 2024
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//---------------------------------------
// Surface class
//---------------------------------------
class Surface 
{
public:
   // constructors
   Surface();
   Surface(int size);
   Surface(const Surface &s);
   ~Surface();

   // methods
   void init_sphere();
   void init_vase();
   void display();
   void display(float scale, float dx, float dy, float dz);

private:
   int Size;
   bool Initialized;
   static const int SIZE = 100;
   float Px[SIZE][SIZE];
   float Py[SIZE][SIZE];
   float Pz[SIZE][SIZE];
   float Nx[SIZE][SIZE];
   float Ny[SIZE][SIZE];
   float Nz[SIZE][SIZE];
   float vertices[SIZE*SIZE][3];
   unsigned int indices[SIZE*SIZE*4];

   void init_normals();
   void init_indices();
};

//---------------------------------------
// Default constructor
//---------------------------------------
Surface::Surface()
{
   Size = SIZE;
   Initialized = false;

   for (int u = 0; u < Size; u++)
   for (int v = 0; v < Size; v++)
   {
      Px[u][v] = Py[u][v] = Pz[u][v] = 0;
      Nx[u][v] = Ny[u][v] = Nz[u][v] = 0;
   }
}

//---------------------------------------
// Parameterized constructor
//---------------------------------------
Surface::Surface(int size)
{
   if (size <= SIZE)
      Size = size;
   else
      Size = SIZE;
   Initialized = false;

   for (int u = 0; u < Size; u++)
   for (int v = 0; v < Size; v++)
   {
      Px[u][v] = Py[u][v] = Pz[u][v] = 0;
      Nx[u][v] = Ny[u][v] = Nz[u][v] = 0;
   }
}

//---------------------------------------
// Copy constructor
//---------------------------------------
Surface::Surface(const Surface &s)
{
   Size = s.Size;
   Initialized = s.Initialized;

   for (int u = 0; u < Size; u++)
   for (int v = 0; v < Size; v++)
   {
      Px[u][v] = s.Px[u][v];
      Py[u][v] = s.Py[u][v];
      Pz[u][v] = s.Pz[u][v];
      Nx[u][v] = s.Nx[u][v];
      Ny[u][v] = s.Ny[u][v];
      Nz[u][v] = s.Nz[u][v];
   }
}

//---------------------------------------
// Destructor function
//---------------------------------------
Surface::~Surface()
{
}

//---------------------------------------
// Calculate surface normals
//---------------------------------------
void Surface::init_normals()
{
   // Find surface normals
   if (!Initialized) return;
   for (int u = 0; u < Size-1; u++)
   for (int v = 0; v < Size-1; v++)
   {
      // Get two tangent vectors
      float Ux = Px[u+1][v] - Px[u][v];
      float Uy = Py[u+1][v] - Py[u][v];
      float Uz = Pz[u+1][v] - Pz[u][v];
      float Vx = Px[u][v+1] - Px[u][v];
      float Vy = Py[u][v+1] - Py[u][v];
      float Vz = Pz[u][v+1] - Pz[u][v];
      
      // Do cross product
      Nx[u][v] = Uy * Vz - Uz * Vy;
      Ny[u][v] = Uz * Vx - Ux * Vz;
      Nz[u][v] = Ux * Vy - Uy * Vx;
      float length = sqrt( 
         Nx[u][v] * Nx[u][v] + 
         Ny[u][v] * Ny[u][v] + 
         Nz[u][v] * Nz[u][v]);
      if (length > 0)
      {
         Nx[u][v] /= length;
         Ny[u][v] /= length;
         Nz[u][v] /= length;
      }
   } 

   // Wrap around values for last col
   for (int u = 0; u < Size; u++)
   {
      Nx[u][Size-1] = Nx[u][0];
      Ny[u][Size-1] = Ny[u][0];
      Nz[u][Size-1] = Nz[u][0];
   }

   // Copy previous values for last row
   for (int v = 0; v < Size; v++)
   {
      Nx[Size-1][v] = Nx[Size-2][v];
      Ny[Size-1][v] = Ny[Size-2][v];
      Nz[Size-1][v] = Nz[Size-2][v];
   }
}

//---------------------------------------
// Initialize indices
//---------------------------------------
void Surface::init_indices()
{
   // Initilize vertices array
   if (!Initialized) return;
   int point = 0;
   for (int u = 0; u < Size; u++)
   for (int v = 0; v < Size; v++)
   {
      vertices[point][0] = Px[u][v];
      vertices[point][1] = Py[u][v];
      vertices[point][2] = Pz[u][v];
      point++;
   }

   // Initilize indices array
   int count = 0;
   for (int u = 0; u < Size-1; u++)
   for (int v = 0; v < Size-1; v++)
   {
      int point = u * Size + v; 
      indices[count++] = point;
      indices[count++] = point + 1;
      indices[count++] = point + Size + 1;
      indices[count++] = point + Size;
   }
}

//---------------------------------------
// Initialize sphere surface
//---------------------------------------
void Surface::init_sphere()
{
   // Initialize sphere surface and normals
   Initialized = true;
   for (int u = 0; u < Size; u++)
   {
      float angle1 = 2 * u * M_PI / (Size - 1);
      for (int v = 0; v < Size; v++)
      {
         float angle2 = v * M_PI / (Size - 1);
         Px[u][v] = Nx[u][v] = sin(angle2) * cos(angle1);
         Py[u][v] = Ny[u][v] = sin(angle2) * sin(angle1);
         Pz[u][v] = Nz[u][v] = cos(angle2);
      }
   }
   init_indices();
}

//---------------------------------------
// Initialize vase surface 
//---------------------------------------
void Surface::init_vase()
{
   // Initialize vase surface and normals
   Initialized = true;
   float radius1 = 0.25;
   float radius2 = 0.75;
   for (int u = 0; u < Size; u++)
   {
      float angle1 = 2 * u * M_PI / (Size - 1);
      for (int v = 0; v < Size; v++)
      {
         float angle2 = 2 * v * M_PI / (Size - 1);
         Px[u][v] = cos(angle1) * (radius2 + radius1*sin(angle2));
         Py[u][v] = sin(angle1) * (radius2 + radius1*sin(angle2));
         Pz[u][v] = 2.0 * v / (Size - 1.0) - 1.0;
      }
   }
   init_normals();
   init_indices();
}

//---------------------------------------
// Display surface coordinates
//---------------------------------------
void Surface::display()
{
   if (!Initialized) return;
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#define FAST_METHOD
#ifdef FAST_METHOD
   // Fast method
   glDrawElements(GL_QUADS, 4*(SIZE-1)*(SIZE-1), GL_UNSIGNED_INT, indices);
#endif

#ifdef MEDIUM_METHOD
   // Medium method
   for(int i = 0; i < (Size-1)*(Size-1); i++)
      glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, &(indices[4*i]));
#endif

#ifdef SLOW_METHOD
   // Slow method
   for (int u = 0; u < Size-1; u++)
      for (int v = 0; v < Size-1; v++)
      {
         glBegin(GL_LINE_LOOP);
         glVertex3f(Px[u][v], Py[u][v], Pz[u][v]);
         glVertex3f(Px[u + 1][v], Py[u + 1][v], Pz[u + 1][v]);
         glVertex3f(Px[u + 1][v + 1], Py[u + 1][v + 1], Pz[u + 1][v + 1]);
         glVertex3f(Px[u][v + 1], Py[u][v + 1], Pz[u][v + 1]);
         glEnd();
      }
#endif
}

//---------------------------------------
// Display transformed surface coordinates 
//---------------------------------------
void Surface::display(float scale, float dx, float dy, float dz)
{
   if (!Initialized) return;
   glPushMatrix();
   glTranslatef(dx, dy, dz);
   glScalef(scale, scale, scale);
   display();
   glPopMatrix();
}

// Global variables
float xangle = 0;
float yangle = 0;
float zangle = 0;
Surface sphere(36);
Surface vase(36);

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);
   glEnableClientState(GL_VERTEX_ARRAY);

   // Initialize surfaces
   sphere.init_sphere();
   vase.init_vase();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'x')
      xangle -= 5;
   else if (key == 'X')
      xangle += 5;
   else if (key == 'y')
      yangle -= 5;
   else if (key == 'Y')
      yangle += 5;
   else if (key == 'z')
      zangle -= 5;
   else if (key == 'Z')
      zangle += 5;
   else if (key == 'q')
      exit(0);

   // Redraw objects
   glutPostRedisplay();
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

   // Draw geometric objects
   sphere.display(0.5, -1, 0, 0);
   vase.display(0.5, 1, 0, 0);
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Surface");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();

   printf("Keyboard commands:\n");
   printf("   'x' - rotate x-axis -5 degrees\n");
   printf("   'X' - rotate x-axis +5 degrees\n");
   printf("   'y' - rotate y-axis -5 degrees\n");
   printf("   'Y' - rotate y-axis +5 degrees\n");
   printf("   'z' - rotate z-axis -5 degrees\n");
   printf("   'Z' - rotate z-axis +5 degrees\n");
   printf("   'q' - quit program\n");
   glutMainLoop();
   return 0;
}

