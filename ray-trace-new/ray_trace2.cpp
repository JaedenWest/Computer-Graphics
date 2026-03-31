//---------------------------------------
// Program: ray_trace.cpp
// Purpose: Demonstrate ray tracing.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Include ray tracing and phong shading classes
#include "ray_classes.h"

// Global variables
#define XDIM 800
#define YDIM 800
#define ZDIM 800
unsigned char image[YDIM][XDIM][3];
float position = -2;
string mode = "phong";

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[], int count)
{
   // Define ray to light source
   Ray3D shadow_ray;
   shadow_ray.set(pt, dir);

   // Check to see ray intersects any sphere
   Point3D point;
   Vector3D normal;
   for (int index=0; index<count; index++)
      if ((index != current) && 
         (sphere[index].get_intersection(shadow_ray, point, normal)))
         return true;
   return false;
}

//---------------------------------------
// Perform ray tracing of scene
//---------------------------------------
void ray_trace()
{
   // Define camera point
   Point3D camera;
   camera.set(0,0,position);

   // Define light source
   ColorRGB light_color;
   light_color.set(250,250,250);
   Vector3D light_dir;
   light_dir.set(-1,-1,-1);
   light_dir.normalize();

   // Define shader
   Phong shader;
   shader.SetCamera(camera);
   shader.SetLight(light_color, light_dir);

   // Define array of spheres
   const int SPHERES = 10;
   Sphere3D sphere[SPHERES];
   ColorRGB color[SPHERES];
   for (int s=0; s<SPHERES; s++)
   {
      float cx = myrand(-2, 2);
      float cy = myrand(-2, 2);
      float cz = myrand(3, 6);
      Point3D center;
      center.set(cx,cy,cz);
      float radius = myrand(0.5,1);
      float reflect = rand() % 100 / 100.0;
      float refract = rand() % 100 / 100.0;
      sphere[s].set(center, radius, reflect, refract);
      int R = rand() % 255;
      int G = rand() % 255;
      int B = rand() % 255;
      color[s].set(R,G,B);
   }

   // Perform ray tracing
   for (int y = 0; y < YDIM; y++)
   for (int x = 0; x < XDIM; x++)
   {
      // Clear image
      image[y][x][0] = 0;
      image[y][x][1] = 0;
      image[y][x][2] = 0;

      // Define sample point on image plane
      float xpos = (x - XDIM/2) * 2.0 / XDIM;
      float ypos = (y - YDIM/2) * 2.0 / YDIM;
      Point3D point;
      point.set(xpos, ypos, 0);

      // Define ray from camera through image
      Ray3D ray;
      ray.set(camera, point);

      // Perform sphere intersection
      int closest = -1;
      Point3D p, closest_p;
      Vector3D n, closest_n;
      closest_p.set(0,0,ZDIM);
      for (int s=0; s<SPHERES; s++)
      {
         if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz))
         {
            closest = s;
            closest_p = p;
            closest_n = n;
         }
      }

      // Calculate pixel color
      if (closest >= 0)
      {
        // Display surface normal
         if (mode == "normal")
         {
            image[y][x][0] = 127 + closest_n.vx * 127;
            image[y][x][1] = 127 + closest_n.vy * 127;
            image[y][x][2] = 127 + closest_n.vz * 127;
         }

         // Calculate Phong shade
         if (mode == "phong")
         {
            ColorRGB final_color;
            final_color.set(0, 0, 0);

            Ray3D current_ray = ray;
            ColorRGB current_color;
            float reflection_factor = 1.0;

            for (int reflection_count = 0; reflection_count < 10; reflection_count++) // Primary ray + 2 reflections
            {
               Point3D p;
               Vector3D n;

               // Check for intersection
               int closest_reflection = -1;
               Point3D closest_reflection_p;
               Vector3D closest_reflection_n;
               closest_reflection_p.set(0, 0, ZDIM);

               for (int s = 0; s < SPHERES; s++)
               {
                  if ((sphere[s].get_intersection(current_ray, p, n)) && (p.pz < closest_reflection_p.pz))
                  {
                     closest_reflection = s;
                     closest_reflection_p = p;
                     closest_reflection_n = n;
                  }
               }

               if (closest_reflection >= 0)
               {
                  // Calculate local Phong shading
                  shader.SetObject(color[closest_reflection], 0.4, 0.4, 0.4, 10);
                  shader.GetShade(closest_reflection_p, closest_reflection_n, current_color);


                  current_color.mult(reflection_factor);
                  final_color.add(current_color);

                  // Calculate reflection direction
                  //𝑅=𝐿+2ℎ=2(𝐿∙𝑛)𝑛 −𝐿
                  Vector3D L = current_ray.dir;
                  L.normalize();
                  float ndotl = closest_reflection_n.dot(L);
                  Vector3D R = closest_reflection_n;
                  R.mult(2 * ndotl);
                  R.sub(L);
                  R.normalize();

                 const float epsilon = 0.001f;
                 Point3D offset_point;
                 offset_point.set(
                    closest_reflection_p.px + R.vx * epsilon,
                    closest_reflection_p.py + R.vy * epsilon,
                    closest_reflection_p.pz + R.vz * epsilon
                 );
                 current_ray.set(offset_point, R);

                  reflection_factor *=  sphere[closest_reflection].reflection;
               }
               else
               {
                  break;
               }
            }

            // Set the final color to the image
            image[y][x][0] = final_color.R;
            image[y][x][1] = final_color.G;
            image[y][x][2] = final_color.B;
         }
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

   // Print command menu
   cout << "Program commands:\n"
        << "   '+' - increase camera distance\n"
        << "   '-' - decrease camera distance\n"
        << "   'p' - show Phong shading\n"
        << "   'n' - show surface normals\n"
        << "   'q' - quit program\n";

   // Perform ray tracing
   cout << "camera: 0,0," << position << endl;
   ray_trace();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // End program
   if (key == 'q')
      exit(0);

   // Move camera position
   else if (key == '+' && position > -5)
   {
      position = position - 0.2;
      cout << "camera: 0,0," << position << endl;
   }
   else if (key == '-' && position < -0.5)
   {
      position = position + 0.2;
      cout << "camera: 0,0," << position << endl;
   }

   // Change display mode
   else if (key == 'n')
      mode = "normal";
   else if (key == 'p')
      mode = "phong";

   // Perform ray tracing
   ray_trace();
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Ray Trace");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}