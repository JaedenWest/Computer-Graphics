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
float angle = 0;
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
    color.set(255, 255, 255);
    Vector3D dir;
    dir.set(-1, -1, -1);
    dir.normalize();
    shader.SetLight(color, dir);

    // Set object properties
    ColorRGB yellow;
    yellow.set(255, 255, 0);
    shader.SetObject(yellow, 0.3, 0.4, 0.4, 2);

    // sky
    ColorRGB sky;
    sky.set(20, 50, 150);

    // first sphere
    Sphere3D sphere;
    Point3D center;
    center.set(0, 0, 3);
    float radius = 1.2;
    sphere.set(center, radius);

    // second sphere
    float sx = sin(angle) * 2;
    float sz = cos(angle) * 2 + 3;
    Sphere3D movingSphere;
    Point3D movingCenter;
    float tMoving;
    Point3D pMoving;
    Vector3D nMoving;
    movingCenter.set(sx, -0.4, sz);
    movingSphere.set(movingCenter, 0.5);

    Plane3D plane;
    Point3D planePoint;
    planePoint.set(0, -2, 0);
    Vector3D planeNormal;
    planeNormal.set(0, 1, 0);
    plane.set(planePoint, planeNormal);
    // Perform ray tracing
    for (int y = 0; y < YDIM; y++)
    {
        for (int x = 0; x < XDIM; x++)
        {

            // Clear image
            image[y][x][0] = sky.R;
            image[y][x][1] = sky.G;
            image[y][x][2] = sky.B;
            float scale = 2.0;

            float xpos = (x - XDIM * 0.5f) * scale / XDIM;
            float ypos = (y - YDIM * 0.5f) * scale / YDIM;

            Point3D point;
            point.set(xpos, ypos, 0);

            // Define ray from camera through image
            Ray3D ray;
            ray.set(camera, point);

            // Perform sphere intersection
            float tSphere, tPlane;
            Point3D pSphere, pPlane;
            Vector3D nSphere;

            bool hitSphere = sphere.get_intersection(ray, tSphere, pSphere, nSphere);
            bool hitPlane = plane.get_intersection(ray, tPlane, pPlane);
            bool hitMoving = movingSphere.get_intersection(ray, tMoving, pMoving, nMoving);
            // nothing hit
            float tMin = 1e9;
            // 1 = sphere 2 = moving sphere 3 = plane
            int hitType = 0;
            // choose the closest object

            if (hitSphere && tSphere < tMin)
            {
                tMin = tSphere;
                hitType = 1;
            }
            if (hitMoving && tMoving < tMin)
            {
                tMin = tMoving;
                hitType = 2;
            }
            if (hitPlane && tPlane < tMin)
            {
                tMin = tPlane;
                hitType = 3;
            }

            ColorRGB shaded;
            if (hitType == 1)
            {
                // display surface normal
                if (mode == "normal")
                {
                    image[y][x][0] = 127 + nSphere.vx * 127;
                    image[y][x][1] = 127 + nSphere.vy * 127;
                    image[y][x][2] = 127 + nSphere.vz * 127;
                }
                else
                {
                    shader.GetShade(pSphere, nSphere, shaded);
                    image[y][x][0] = shaded.R;
                    image[y][x][1] = shaded.G;
                    image[y][x][2] = shaded.B;
                }
            }
            else if (hitType == 2)
            {
                // display surface normal
                if (mode == "normal")
                {
                    image[y][x][0] = 127 + nMoving.vx * 127;
                    image[y][x][1] = 127 + nMoving.vy * 127;
                    image[y][x][2] = 127 + nMoving.vz * 127;
                }
                else
                {
                    shader.GetShade(pMoving, nMoving, shaded);
                    image[y][x][0] = shaded.R;
                    image[y][x][1] = shaded.G;
                    image[y][x][2] = shaded.B;
                }
            }
            else if (hitType == 3)
            {
                int checkSize = 1;

                float fx = pPlane.px * 0.3f;
                float fz = pPlane.pz * 0.3f;

                int checkX = (int)floor(fx / checkSize);
                int checkZ = (int)floor(fz / checkSize);

                bool even = ((checkX + checkZ) % 2 == 0);

                ColorRGB checkerColor;

                if (even)
                {
                    checkerColor.set(255, 255, 0);
                }
                else
                {
                    checkerColor.set(220, 40, 40);
                }
                image[y][x][0] = checkerColor.R;
                image[y][x][1] = checkerColor.G;
                image[y][x][2] = checkerColor.B;
            }
            else
            {

                image[y][x][0] = sky.R;
                image[y][x][1] = sky.G;
                image[y][x][2] = sky.B;
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
    ray_trace();
}
//---------------------------------------
// Idle function for OpenGL
//---------------------------------------
void idle()
{
    angle += 0.02; // controls speed of motion

    ray_trace(); // recompute image
    glutPostRedisplay();
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
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
