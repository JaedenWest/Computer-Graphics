//---------------------------------------
// Program: Homework5.h
// Purpose: Ray Tracing header file
// Author:  Jaeden West
// Date:    4/10/2026
//---------------------------------------

#include <iostream>
#include <cmath>
using namespace std;

#ifndef _HOMEWORK5_CLASSES_H_
#define _HOMEWORK5_CLASSES_H_

// Used to make it easier to test and debug
const bool DEBUG = false;

// For colors
class ColorRGB
{
public:
    float R, G, B;
    // Used for debugging
    string print();

    // Set color values
    void set(float r, float g, float b);
    // Darkens or lightens the color
    void mult(float c);
    // Combines object color with light color
    // Used for diffuse and specular shading
    void mult(ColorRGB c);
    // Used for combining multiple light sources or reflections
    void add(ColorRGB c);
    void sub(ColorRGB c);
    // Makes sure values are between 0-1
    void clamp();
};
// A position in 3D space
class Point3D
{
public:
    // A location in the space
    float px, py, pz;
    // Used for debugging
    string print();

    // Setting the location
    void set(float x, float y, float z);
    // Used for comparing distance or how far away two objects are from each other
    float distance(Point3D p);
};

// Repesents direction and magnitude of movement
class Vector3D
{
public:
    // Where you are and where are you moving
    float vx, vy, vz;
    void set(float x, float y, float z);
    // Used for debugging
    string print();

    // used in lighting so direction is consistent
    void normalize();
    // measures the angle similarity
    float dot(Vector3D v);

    // Darkens or lightens the color
    void mult(float c);
    // Used for combining multiple light sources or reflections
    void add(Vector3D v);
    void sub(Vector3D v);
};

class Ray3D
{
public:
    // Used for debugging
    string print();

    Point3D point;
    Vector3D dir;
    // Set for origin and direction of ray
    void set(Point3D p, Vector3D d);
    // set using two points, one for origin and one for direction si p1 is where we started and p2 is where we are going
    void set(Point3D p1, Point3D p2);
    // sample point along the ray at distance t from the origin
    // used for finding intersection points and reflection rays
    Point3D get_sample(float t);
};
// object in scene
class Sphere3D
{
public:
    // Used for debugging
    string print();

    Point3D center;
    Vector3D motion;
    float radius;
    // create a static or moving sphere
    void set(Point3D p, float r);
    void set(Point3D p, float r, Vector3D m);

    // intersection test
    // does this ray hit the sphere? if so, where and what is the normal vector at the intersection point?
    bool get_intersection(Ray3D ray, float &t, Point3D &point, Vector3D &normal);
};

class Plane3D
{
public:
    // used for debugging
    string print();

    Point3D point;
    Vector3D normal;

    void set(Point3D p, Vector3D n);
    bool get_intersection(Ray3D ray, float &t, Point3D &hit);
};

class Cube3D
{
public:
    Point3D minP; // bottom-left-back corner
    Point3D maxP; // top-right-front corner

    void set(Point3D minPoint, Point3D maxPoint);

    bool get_intersection(Ray3D ray, float &t, Point3D &hitPoint, Vector3D &normal);
};

// lighting system
class Phong
{
public:
    // constructors
    Phong();
    ~Phong();

    // set methods
    void SetCamera(Point3D pos);
    void SetLight(ColorRGB color, Vector3D dir);
    void SetObject(ColorRGB color, float ka, float kd, float ks, float kp);
    // gets the final color of the object being shaded
    void GetShade(Point3D point, Vector3D normal, ColorRGB &color);

private:
    // Where the camera is
    Point3D CameraPos;

    // Color and direction of the light
    ColorRGB LightColor;
    Vector3D LightDir;

    // Base color of the object
    ColorRGB ObjectColor;
    // Ambient, diffuse, specular, and phong exponent values for the object
    float Ka, Kd, Ks, Kp;
};

#endif