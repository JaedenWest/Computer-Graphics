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
float cubeAngle = angle;
float radius = 0.8f;
string mode = "phong";

Cube3D cube1;
Cube3D cube2;
ColorRGB trace_ray(Ray3D ray, int depth)
{
    ColorRGB sky;
    sky.set(20, 50, 150);

    struct Material
    {
        ColorRGB color;
        float ka, kd, ks, n;
    };

    Material sphereMat, movingMat, cube1Mat, cube2Mat;

    sphereMat.color.set(0, 255, 0);
    sphereMat.ka = 0.2;
    sphereMat.kd = 0.6;
    sphereMat.ks = 0.4;
    sphereMat.n = 20;

    movingMat.color.set(255, 255, 0);
    movingMat.ka = 0.3;
    movingMat.kd = 0.7;
    movingMat.ks = 0.3;
    movingMat.n = 20;

    cube1Mat.color.set(255, 0, 0);
    cube1Mat.ka = 0.3;
    cube1Mat.kd = 0.5;
    cube1Mat.ks = 0.5;
    cube1Mat.n = 30;

    cube2Mat.color.set(0, 0, 255);
    cube2Mat.ka = 0.3;
    cube2Mat.kd = 0.6;
    cube2Mat.ks = 0.4;
    cube2Mat.n = 15;

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

    // cube 1 center motion
    float c1x = -1.0f + cos(cubeAngle) * radius;
    float c1z = 3.0f + sin(cubeAngle) * radius;

    // cube 2 center motion (opposite side)
    float c2x = 1.0f + cos(cubeAngle + 3.14f) * radius;
    float c2z = 3.0f + sin(cubeAngle + 3.14f) * radius;

    float size = 0.3f;
    // cube 1
    Point3D c1Min, c1Max;
    c1Min.set(c1x - size, 1.4f, c1z - size);
    c1Max.set(c1x + size, 2.0f, c1z + size);
    cube1.set(c1Min, c1Max);

    // cube 2
    Point3D c2Min, c2Max;
    c2Min.set(c2x - size, 1.4f, c2z - size);
    c2Max.set(c2x + size, 2.0f, c2z + size);
    cube2.set(c2Min, c2Max);

    bool hitSphere = sphere.get_intersection(ray, tSphere, pSphere, nSphere);
    bool hitMoving = movingSphere.get_intersection(ray, tMoving, pMoving, nMoving);
    bool hitPlane = plane.get_intersection(ray, tPlane, pPlane);

    float tCube1, tCube2;
    Point3D pCube1, pCube2;
    Vector3D nCube1, nCube2;

    bool hitCube1 = cube1.get_intersection(ray, tCube1, pCube1, nCube1);
    bool hitCube2 = cube2.get_intersection(ray, tCube2, pCube2, nCube2);

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
    if (hitCube1 && tCube1 < tMin)
    {
        tMin = tCube1;
        hitType = 4;
    }

    if (hitCube2 && tCube2 < tMin)
    {
        tMin = tCube2;
        hitType = 5;
    }
    if (hitType == 0)
        return sky;

    Point3D hitPoint;
    Vector3D normal;
    Material mat;
    ColorRGB objColor;
    objColor.set(255, 255, 0);

    if (hitType == 1)
    {
        hitPoint = pSphere;
        normal = nSphere;
        mat = sphereMat;
    }
    else if (hitType == 2)
    {
        hitPoint = pMoving;
        normal = nMoving;
        mat = movingMat;
    }
    else if (hitType == 3)
    {
        hitPoint = pPlane;
        normal = planeNormal;
        normal.normalize();

        bool inBounds =
            (pPlane.px > -13 && pPlane.px < 50 &&
             pPlane.pz > 0 && pPlane.pz < 75);

        if (!inBounds)
            return sky;

        int checkSize = 1;
        int checkX = (int)floor(pPlane.px / checkSize);
        int checkZ = (int)floor(pPlane.pz / checkSize);

        bool even = ((checkX + checkZ) % 2 == 0);

        if (even)
            mat.color.set(255, 255, 0);
        else
            mat.color.set(220, 40, 40);

        mat.ka = 0.3;
        mat.kd = 0.7;
        mat.ks = 0.3;
        mat.n = 10;
    }
    else if (hitType == 4)
    {
        hitPoint = pCube1;
        normal = nCube1;
        mat = cube1Mat;
    }
    else if (hitType == 5)
    {
        hitPoint = pCube2;
        normal = nCube2;
        mat = cube2Mat;
    }

    normal.normalize();

    Phong shader;

    Point3D camera;
    camera.set(0, 0, -position);
    shader.SetCamera(camera);

    ColorRGB lightColor;
    lightColor.set(255, 255, 255);

    Vector3D lightDir;
    lightDir.set(-0.6, 0.8, -0.3);
    lightDir.normalize();

    shader.SetLight(lightColor, lightDir);
    shader.SetObject(mat.color, mat.ka, mat.kd, mat.ks, mat.n);

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

    float ks = mat.ks;

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
    cubeAngle += 0.04; // controls cube speed
    angle += 0.04;     // controls spheres speed

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
