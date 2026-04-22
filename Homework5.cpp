//---------------------------------------
// Program: Homework5.cpp
// Purpose: Ray Tracing
// Author: Jaeden West
// Date: 4/10/2026
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

ColorRGB trace_ray(Ray3D ray, int depth)
{
    ColorRGB sky;
    sky.set(20, 50, 150);

    if (depth >= 3)
        return sky;

    Sphere3D sphere;
    Point3D center;
    center.set(0, 0, 3);
    sphere.set(center, 1.2);

    float sx = sin(angle) * 2;
    float sz = cos(angle) * 2 + 3;

    Sphere3D movingSphere;
    Point3D movingCenter;
    movingCenter.set(sx, -0.4, sz);
    movingSphere.set(movingCenter, 0.8);

    Plane3D plane;
    Point3D planePoint;
    planePoint.set(0, -4, 0);

    Vector3D planeNormal;
    planeNormal.set(0, 1, 0);
    plane.set(planePoint, planeNormal);

    float tSphere, tMoving, tPlane;
    Point3D pSphere, pMoving, pPlane;
    Vector3D nSphere, nMoving;

    bool hitSphere = sphere.get_intersection(ray, tSphere, pSphere, nSphere);
    bool hitMoving = movingSphere.get_intersection(ray, tMoving, pMoving, nMoving);
    bool hitPlane = plane.get_intersection(ray, tPlane, pPlane);

    float tMin = 1e9;
    int hitType = 0;

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

    if (hitType == 0)
        return sky;

    Point3D hitPoint;
    Vector3D normal;

    ColorRGB objColor;
    objColor.set(255, 255, 0);

    if (hitType == 1)
    {
        hitPoint = pSphere;
        normal = nSphere;
    }
    else if (hitType == 2)
    {
        hitPoint = pMoving;
        normal = nMoving;
    }
    else if (hitType == 3)
    {
        hitPoint = pPlane;
        normal = planeNormal;

        bool inBounds =
            (pPlane.px > -13 && pPlane.px < 50 &&
             pPlane.pz > 0 && pPlane.pz < 75);

        if (!inBounds)
            return sky;

        int checkSize = 1;
        int checkX = (int)floor(pPlane.px / checkSize);
        int checkZ = (int)floor(pPlane.pz / checkSize);

        bool even = ((checkX + checkZ) % 2 == 0);

        ColorRGB checkerColor;
        if (even)
            checkerColor.set(255, 255, 0);
        else
            checkerColor.set(220, 40, 40);

        return checkerColor;
    }

    normal.normalize();

    Phong shader;

    Point3D camera;
    camera.set(0, 0, -position);
    shader.SetCamera(camera);

    ColorRGB lightColor;
    lightColor.set(255, 255, 255);

    Vector3D lightDir;
    lightDir.set(-0.6, -0.8, -0.3);
    lightDir.normalize();

    shader.SetLight(lightColor, lightDir);
    shader.SetObject(objColor, 0.3, 0.4, 0.4, 10);

    ColorRGB phongColor;
    shader.GetShade(hitPoint, normal, phongColor);

    // Reflection
    Vector3D D = ray.dir;
    Vector3D N = normal;

    float dot = D.dot(N);

    Vector3D R;
    R.vx = D.vx - 2 * dot * N.vx;
    R.vy = D.vy - 2 * dot * N.vy;
    R.vz = D.vz - 2 * dot * N.vz;
    R.normalize();

    Point3D offset;
    offset.px = hitPoint.px + R.vx * 0.001f;
    offset.py = hitPoint.py + R.vy * 0.001f;
    offset.pz = hitPoint.pz + R.vz * 0.001f;

    Point3D target;
    target.px = offset.px + R.vx;
    target.py = offset.py + R.vy;
    target.pz = offset.pz + R.vz;

    Ray3D reflectedRay;
    reflectedRay.set(offset, target);

    ColorRGB reflectedColor = trace_ray(reflectedRay, depth + 1);

    float ks = 0.3f;

    ColorRGB finalColor;
    finalColor.R = phongColor.R + reflectedColor.R * ks;
    finalColor.G = phongColor.G + reflectedColor.G * ks;
    finalColor.B = phongColor.B + reflectedColor.B * ks;

    finalColor.R = min(255.0f, max(0.0f, finalColor.R));
    finalColor.G = min(255.0f, max(0.0f, finalColor.G));
    finalColor.B = min(255.0f, max(0.0f, finalColor.B));

    return finalColor;
}
void ray_trace()
{
    // sky
    ColorRGB sky;
    sky.set(20, 50, 150);

    // camera
    Point3D camera;
    camera.set(0, 0, -position);

    // image plane scale
    float scale = 2.0;

    // Perform ray tracing
    for (int y = 0; y < YDIM; y++)
    {
        for (int x = 0; x < XDIM; x++)
        {

            // Clear image
            image[y][x][0] = sky.R;
            image[y][x][1] = sky.G;
            image[y][x][2] = sky.B;

            float xpos = (x - XDIM * 0.5f) * scale / XDIM;
            float ypos = (y - YDIM * 0.5f) * scale / YDIM;

            Point3D point;
            point.set(xpos, ypos, 0);

            // Define ray from camera through image
            Ray3D ray;
            ray.set(camera, point);

            ColorRGB col = trace_ray(ray, 0);

            image[y][x][0] = col.R;
            image[y][x][1] = col.G;
            image[y][x][2] = col.B;
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
    angle += 0.03; // controls speed of motion

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
