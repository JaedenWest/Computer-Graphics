//---------------------------------------
// Program: Homework5.cpp
// Purpose: Ray Tracing
// Author:  Jaeden West
// Date:    4/10/2026
//---------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Homework5_classes.h"

// Global variables
#define XDIM 800
#define YDIM 800
unsigned char image[YDIM][XDIM][3];
float position = 2;
string mode = "phong";

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void ray_trace()
{
    // Define Phong shader
    Phong shader;

    // Define camera point
    Point3D camera;
    camera.set(0, 0, -position);
    shader.SetCamera(camera);

    // Define light source
    ColorRGB color;
    color.set(200, 200, 200);
    Vector3D dir;
    dir.set(-1, -1, -1);
    dir.normalize();
    shader.SetLight(color, dir);

    // Set object properties
    color.set(200, 0, 100);
    shader.SetObject(color, 0.3, 0.4, 0.4, 10);

    // Define test sphere
    Sphere3D sphere;
    Point3D center;
    ColorRGB sky;
    center.set(0, 0, 3);
    float radius = 2;
    sphere.set(center, radius);
    sky.set(20, 50, 150);

    // Perform ray tracing
    for (int y = 0; y < YDIM; y++)
    {
        for (int x = 0; x < XDIM; x++)
        {
            // Clear image
            image[y][x][0] = sky.R;
            image[y][x][1] = sky.G;
            image[y][x][2] = sky.B;

            // Define sample point on image plane
            float xpos = (x - XDIM / 2) * 2.0 / XDIM;
            float ypos = (y - YDIM / 2) * 2.0 / YDIM;
            Point3D point;
            point.set(xpos, ypos, 0);

            // Define ray from camera through image
            Ray3D ray;
            ray.set(camera, point);

            // Perform sphere intersection
            Point3D planeHit;
            Vector3D planeNormal;
            bool hitPlane = false;
            float tPlane = 0;

            float denom = ray.d.vy;
            if (sphere.get_intersection(ray, planeHit, planeNormal))
            {
                // Display surface normal
                if (mode == "normal")
                {
                    image[y][x][0] = 127 + planeNormal.vx * 127;
                    image[y][x][1] = 127 + planeNormal.vy * 127;
                    image[y][x][2] = 127 + planeNormal.vz * 127;
                }

                // Calculate Phong shade
                if (mode == "phong")
                {
                    shader.GetShade(planeHit, planeNormal, color);
                    image[y][x][0] = color.R;
                    image[y][x][1] = color.G;
                    image[y][x][2] = color.B;
                }
            }
        }
    }
    bool intersectPlane(Ray3D ray, float &t, Point3D &hit)
    {
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
    else if (key == '+' && position < 5)
        position = position * 1.1;
    else if (key == '-' && position > 1)
        position = position / 1.1;

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
