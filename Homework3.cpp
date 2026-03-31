//Program: Homework3 Terrain Models
//Purpose: Implement terrain models that has a birds eye view, as well using shading and color interpolation
//Author: Jaeden West

//Standard Libraries
#include <ctime>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

//OpenGl glut headers for mac computer 
#ifdef Mac
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif
using namespace std;

//rendering mode used when drawing terrain
//we do GL_LINE_LOOP as it draws the edges of each quad which makes it easier to visualize
GLenum mode = GL_POLYGON;

//size of the terrain grid SIZE * SIZE 
const int SIZE = 50;

//Map Class making a terrain surface of a grid of vertices
class Map
{
public:
   // Material properties
   float Ka = 0.5;
   float Kd = 0.8;   
   float Ks = 0.8;
   float Kp = 50;
   
   //Arrays for storing the coords of each vertex in the grid
   //Pz is the height(depth) of terrain
   float Px[SIZE][SIZE];
   float Py[SIZE][SIZE];
   float Pz[SIZE][SIZE];

   //Arrays for storing the RGB of each vertex at different height
   float Cr[SIZE][SIZE];
   float Cg[SIZE][SIZE];
   float Cb[SIZE][SIZE];
   //surface normals
   //these are used for lighting calculations 
   float Nx[SIZE][SIZE];
   float Ny[SIZE][SIZE];
   float Nz[SIZE][SIZE];
    
   //texture image and we do the pointer since we don't know how big the image is
   //texture is stored as a contiguous(touching) block of pixel data
   unsigned char *texture;
   //dimensions of the texture image
   int xdim, ydim;
   //draw function to render the vertices of the terrain
   void draw() const
   {  
      //nested for loop to go through every array cord and create the square cell
      //each cell has 4 vertices
      for(int i = 0; i < SIZE - 1; i++)
      {
         for(int j = 0; j < SIZE - 1; j++)
         {

            //begin drawing a polygon for the current cell
            glBegin(mode);
            //bottom left
            //set color for terrain
            glColor3f(Cr[i][j], Cg[i][j], Cb[i][j]);
            glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
            glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
            //bottom right
            //set color for terrain
            glColor3f(Cr[i + 1][j], Cg[i + 1][j], Cb[i + 1][j]);
            glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
            glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
            //top right
            //set color for terrain
            glColor3f(Cr[i + 1][j + 1], Cg[i + 1][j + 1], Cb[i + 1][j + 1]);
            glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
            glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
            //top right
            //set color for terrain
            glColor3f(Cr[i][j + 1], Cg[i][j + 1], Cb[i][j + 1]); 
            glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]); 
            glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
            //ends the polygon for the current cell
            glEnd();
         }
      }
   }

   //---------------------------------------
   // Initialize material properties
   //---------------------------------------
   void init_material(float Ka, float Kd, float Ks, float Kp,
                     float Mr, float Mg, float Mb)
   {
      // Material variables
      float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
      float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
      float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };

      // Initialize material
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
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
   // Initialize light source
   //---------------------------------------
   void init_light(int light_source, float Lx, float Ly, float Lz, 
   float Lr, float Lg, float Lb)
   {
      // Light variables
      float light_position[] = { Lx, Ly, Lz, 0.0 };
      float light_color[] = { Lr, Lg, Lb, 1.0 };

      // Initialize light source
      glEnable(GL_LIGHTING);
      glEnable(light_source);
      glLightfv(light_source, GL_POSITION, light_position);
      glLightfv(light_source, GL_AMBIENT, light_color);
      glLightfv(light_source, GL_DIFFUSE, light_color);
      glLightfv(light_source, GL_SPECULAR, light_color);
      glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
      glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
      glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   }
   //used to average out the noise so the the hills and valleys are smoother
   void smoothing()
   {
      //temp terrain to store values
      float temp[SIZE][SIZE];

      //loop through interior vertices skipping edges
      for(int i = 1; i < SIZE - 1; i++)
      {
         for(int j = 1; j < SIZE - 1; j++)
         {
            //used to increment what row and column im on
            int count = 0;
            //stores the average of each 3x3
            float sum = 0;
            //loop for the 3x3 averaging
            for(int i2 = -1; i2 <= 1; i2++)
            {
               for(int j2 = -1; j2 <= 1; j2++)
               {
                  sum += Pz[i + i2][j + j2];
                  count++;
                  
               }
            }   
         temp[i][j] = sum / count; 
         }
      }
      //copy smoothing values back
      for(int i = 1; i < SIZE - 1; i++)
      {
         for(int j = 1; j < SIZE - 1; j++)
         {
            Pz[i][j] = temp[i][j];
         }
      }

   }
   //used for coloring terrain based on height interpolation
   void color(float h, float &r, float &g, float &b, float minHeight, float maxHeight) const
   {
      //converts h into normalized value between 0-1
      float t = (h - minHeight) / (maxHeight - minHeight);
      if(t < 0.4)
      {
         //set terrain to green to brown
         r = 0.0 * (1 - t) + 0.6 * t;
         g = 0.4 * (1 - t) + 0.4 * t;
         b = 0.0 * (1 - t) * 0.2 * t;
      }
      else if(t < 0.7)
      {
         //remaps 0.5-0.8 to 0-1
         float t_mid = (t - 0.5) / 0.3;
         //set terrain from brown to darker brown
         r = 0.8 * (1 - t_mid) + 0.4 * t_mid;
         g = 0.6 * (1 - t_mid) + 0.3 * t_mid;
         b = 0.3 * (1 - t_mid) + 0.1 * t_mid;
      }
      else
      {
         //set terrain to brown to white
         t = (t - 0.5) * 2;
         //it goes value = start * (1 - progress) + end * progress
         //starts brown, progress to white, ends white
         r = 0.4 * (1 - t) + 1.0*t;
         g = 0.3 * (1 - t) + 1.0*t;
         b = 0.1 * (1 - t) + 1.0*t;
      }
   }
};



//Global Variables
//for rotating with keyboard
float xangle = 0;
float yangle = 0;
float zangle = 0;



//instance of the map class that stores our terrain surface
Map rectangle;


void display_map()
{

}

//initializes the openGl settings and creates the terrain surface
void init()
{
   srand(time(NULL));
   //sets background color (black)
   glClearColor(0.0, 0.0, 0.0, 1.0);
   //switch to projection matrix
   glMatrixMode(GL_PROJECTION);
   //resets the project matrix
   glLoadIdentity();
   //defines the ortho viewing volume or how big the visible coord range is
   glOrtho(-3.0, 3.0, -3.0, 3.0, -3.0, 3.0);
   //enable depth testing so closer objects hide farther ones
   glEnable(GL_DEPTH_TEST);
   //smooth shading
   glShadeModel(GL_SMOOTH);
   //turns on lighting calculations
   glEnable(GL_LIGHTING);
   //enables a light source
   glEnable(GL_LIGHT0);
   //ensures normals are normalized after
   glEnable(GL_NORMALIZE);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   // Initialize smooth shading
   rectangle.init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);

   GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0}; // directional light
   GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    //initialize the terrain surface coords
   for(int i = 0; i < SIZE; i++){
      for(int j = 0; j < SIZE; j++){
         //to make the mountains a little more random
         float noise = ((rand() % 1000) / 1000.0f - 0.5f) * 0.8;
         //map grid indices into coor space of [-1, 1]
         rectangle.Px[i][j] = i * 2.0 / (SIZE - 1.0) - 1.0;
         rectangle.Py[i][j] = j * 2.0 / (SIZE - 1.0) - 1.0;
         //height of the terrain using sin and cos to make a wavy terrain surface
         //0.3 * sin(i / 2.0) + 0.2 is rolling hills
         //noise * cos(j / 3.0) the big hills and small hills
         // 0.1 * sin(i + j / 2.0) diagonal terrain ridges
         rectangle.Pz[i][j] =  0.3 * sin(i / 2.0) + 0.2 + noise * cos(j / 30.0) + 0.1 * sin(i + j / 2.0) + noise;
      }
   }
      //smoothes the terrain
   rectangle.smoothing();
   rectangle.smoothing();


   rectangle.init_normals();
   //computes min and max height
   float minHeight = rectangle.Pz[0][0];
   float maxHeight = rectangle.Pz[0][0];

   for(int i = 0; i < SIZE; i++)
   {
      for(int j = 0; j < SIZE; j++)
      {
         if(rectangle.Pz[i][j] < minHeight) minHeight = rectangle.Pz[i][j];
         if(rectangle.Pz[i][j] > maxHeight) maxHeight = rectangle.Pz[i][j];
      }
   }

   //assign colors based on height
   for(int i = 0; i < SIZE; i++)
   {
      for(int j = 0; j < SIZE; j++)
      {
         rectangle.color(rectangle.Pz[i][j], rectangle.Cr[i][j], rectangle.Cg[i][j], rectangle.Cb[i][j], minHeight, maxHeight);
      }
   }
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'x')
   {
      xangle -= 5;
   }
      else if (key == 'X')
   {
      xangle += 5;
   }
      else if (key == 'y')
   {
      yangle -= 5;
   }
      else if (key == 'Y')
   {
      yangle += 5;
   }
      else if (key == 'z')
   {
      zangle -= 5;
   }
      else if (key == 'Z')
   {
      zangle += 5;
   }

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
//renders the screen every frame
void display()
{
   //clear the screen and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //switch to model-view matrix
   glMatrixMode(GL_MODELVIEW);
   //resets the matrix
   glLoadIdentity();
   //rotates the terrain to view it better
   glRotatef(xangle, 1, 0, 0);
   glRotatef(yangle, 0, 1, 0);
   glRotatef(zangle, 0, 0, 1);

   //draws the terrain surface
   rectangle.draw();
   
   //flush the rendering commands
   glFlush();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   //initialize glut
   glutInit(&argc, argv);
   //sets window size
   glutInitWindowSize(500, 500);
   //sets starting window position
   glutInitWindowPosition(250, 250);
   //set display mode
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   //creates the openGl window and names it
   glutCreateWindow("Terrain Models");
   //register callback functions
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   //initialize the scene
   init();
   //start the glut event loop
   glutMainLoop();
   return 0;
}
