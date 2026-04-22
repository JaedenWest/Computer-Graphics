//---------------------------------------
// Program: Homework5_classes.cpp
// Purpose: Ray Tracing classses file
// Author:  Jaeden West
// Date:    4/10/2026
//---------------------------------------

#include "Homework5_classes.h"

////////////////////////////////////
//          COLORRGB CLASS
////////////////////////////////////
void ColorRGB::set(float r, float g, float b)
{
    R = r;
    G = g;
    B = b;
    clamp();
}

string ColorRGB::print()
{
    cout << "(" << int(R) << "," << int(G) << "," << int(B) << ")";
    return "";
}

// Darkens or lightens the color
void ColorRGB::mult(float c)
{
    R *= c;
    G *= c;
    B *= c;
    clamp();
}
// Combines object color with light color
// Used for diffuse and specular shading
void ColorRGB::mult(ColorRGB c)
{
    R *= c.R / 255;
    G *= c.G / 255;
    B *= c.B / 255;
    clamp();
}
// Used for combining multiple light sources or reflections
void ColorRGB::add(ColorRGB c)
{
    R += c.R;
    G += c.G;
    B += c.B;
    clamp();
}
void ColorRGB::sub(ColorRGB c)
{
    R -= c.R;
    G -= c.G;
    B -= c.B;
    clamp();
}
void ColorRGB::clamp()
{
    if (R < 0)
        R = 0;
    if (G < 0)
        G = 0;
    if (B < 0)
        B = 0;
    if (R > 255)
        R = 255;
    if (G > 255)
        G = 255;
    if (B > 255)
        B = 255;
}

////////////////////////////////////
//          Point3D CLASS
////////////////////////////////////

// Used for debugging
string Point3D::print()
{
    cout << "(" << px << "," << py << "," << pz << ")";
    return "";
}

// Setting the location
void Point3D::set(float x, float y, float z)
{
    px = x;
    py = y;
    pz = z;
}
// Used for comparing distance or how far away two objects are from each other
float Point3D::distance(Point3D p)
{
    float dx = px - p.px;
    float dy = py - p.py;
    float dz = pz - p.pz;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

////////////////////////////////////
//          Vector3D CLASS
////////////////////////////////////

// Repesents direction and magnitude of movement

// Used for debugging
string Vector3D::print()
{
    cout << "(" << vx << "," << vy << "," << vz << ")";
    return "";
}

void Vector3D::set(float x, float y, float z)
{
    vx = x;
    vy = y;
    vz = z;
}

// used in lighting so direction is consistent
void Vector3D::normalize()
{
    float length = sqrt(vx * vx + vy * vy + vz * vz);
    vx /= length;
    vy /= length;
    vz /= length;
}
// measures the angle similarity
float Vector3D::dot(Vector3D v)
{
    return vx * v.vx + vy * v.vy + vz * v.vz;
}

// Darkens or lightens the color
void Vector3D::mult(float c)
{
    vx *= c;
    vy *= c;
    vz *= c;
}
// Used for combining multiple light sources or reflections
void Vector3D::add(Vector3D v)
{
    vx += v.vx;
    vy += v.vy;
    vz += v.vz;
}
void Vector3D::sub(Vector3D v)
{
    vx -= v.vx;
    vy -= v.vy;
    vz -= v.vz;
}

////////////////////////////////////
//          Ray3D CLASS
////////////////////////////////////

// Used for debugging
string Ray3D::print()
{
    point.print();
    cout << " ";
    dir.print();
    return "";
}

// Set for origin and direction of ray
void Ray3D::set(Point3D p, Vector3D d)
{
    point = p;
    dir = d;
    dir.normalize();
}
// set using two points, one for origin and one for direction si p1 is where we started and p2 is where we are going
void Ray3D::set(Point3D p1, Point3D p2)
{
    point = p1;
    dir.set(p2.px - p1.px, p2.py - p1.py, p2.pz - p1.pz);
    dir.normalize();
}
// sample point along the ray at distance t from the origin
// used for finding intersection points and reflection rays
Point3D Ray3D::get_sample(float t)
{
    Point3D result;
    result.set(point.px + t * dir.vx, point.py + t * dir.vy, point.pz + t * dir.vz);
    return result;
}

////////////////////////////////////
//          Sphere3D CLASS
////////////////////////////////////

// object in scene

// Used for debugging
string Sphere3D::print()
{
    center.print();
    cout << " ";
    motion.print();
    cout << " " << radius << endl;
    return "";
}

// create a static or moving sphere
void Sphere3D::set(Point3D p, float r)
{
    center = p;
    radius = r;
}
void Sphere3D::set(Point3D p, float r, Vector3D m)
{
    center = p;
    motion = m;
    radius = r;
}

// intersection test
// does this ray hit the sphere? if so, where and what is the normal vector at the intersection point?
bool Sphere3D::get_intersection(Ray3D ray, float &t, Point3D &point, Vector3D &normal)
{
    // define oc vector
    Point3D g = center;
    Point3D p = ray.point;
    Vector3D oc;
    oc.set(p.px - g.px, p.py - g.py, p.pz - g.pz);

    // calculate quadratic equation
    float A = ray.dir.dot(ray.dir);
    float B = 2 * oc.dot(ray.dir);
    float C = oc.dot(oc) - radius * radius;

    // solve quadratic equation for intersection points
    float discriminant = B * B - 4 * A * C;
    if (discriminant >= 0)
    {
        // calculate rwo roots
        float root1 = (-B - sqrt(discriminant)) / (2 * A);
        float root2 = (-B + sqrt(discriminant)) / (2 * A);

        float solution;

        if (root1 > 0 && root2 > 0)
            solution = (root1 < root2) ? root1 : root2;
        else if (root1 > 0)
            solution = root1;
        else if (root2 > 0)
            solution = root2;
        else
            return false;

        t = solution;
        point = ray.get_sample(t);

        // Get surface normal
        normal.set(point.px - center.px, point.py - center.py, point.pz - center.pz);
        normal.normalize();
        return true;
    }
    return false;
}

////////////////////////////////////
//          Plane3D CLASS
////////////////////////////////////

void Plane3D::set(Point3D p, Vector3D n)
{
    point = p;
    normal = n;
    normal.normalize();
}
bool Plane3D::get_intersection(Ray3D ray, float &t, Point3D &hitPoint)
{
    float denom = normal.dot(ray.dir);

    if (fabs(denom) < 0.0001)
        return false;

    // compute the vector from the ray origin to the plane
    Vector3D diff;
    diff.set(point.px - ray.point.px, point.py - ray.point.py, point.pz - ray.point.pz);
    t = diff.dot(normal) / denom;
    // if intersection is behind the camera
    if (t < 0)
        return false;
    hitPoint = ray.get_sample(t);

    return true;
}

////////////////////////////////////
//          Cube3D CLASS
////////////////////////////////////
void Cube3D::set(Point3D minPoint, Point3D maxPoint)
{
    minP = minPoint;
    maxP = maxPoint;
}

bool Cube3D::get_intersection(Ray3D ray, float &t, Point3D &hitPoint, Vector3D &normal)
{
    float tmin = -1e9;
    float tmax = 1e9;

    Point3D rayOrigin = ray.point;
    Vector3D rayDirection = ray.dir;

    // X
    if (fabs(rayDirection.vx) < 1e-6)
    {
        if (rayOrigin.px < minP.px || rayOrigin.px > maxP.px)
            return false;
    }
    else
    {
        float tMinX = (minP.px - rayOrigin.px) / rayDirection.vx;
        float tMaxX = (maxP.px - rayOrigin.px) / rayDirection.vx;

        if (tMinX > tMaxX)
            std::swap(tMinX, tMaxX);

        tmin = std::max(tmin, tMinX);
        tmax = std::min(tmax, tMaxX);

        if (tmin > tmax)
            return false;
    }

    // Y
    if (fabs(rayDirection.vy) < 1e-6)
    {
        if (rayOrigin.py < minP.py || rayOrigin.py > maxP.py)
            return false;
    }
    else
    {
        float tMinY = (minP.py - rayOrigin.py) / rayDirection.vy;
        float tMaxY = (maxP.py - rayOrigin.py) / rayDirection.vy;

        if (tMinY > tMaxY)
            std::swap(tMinY, tMaxY);

        tmin = std::max(tmin, tMinY);
        tmax = std::min(tmax, tMaxY);

        if (tmin > tmax)
            return false;
    }

    // Z
    if (fabs(rayDirection.vz) < 1e-6)
    {
        if (rayOrigin.pz < minP.pz || rayOrigin.pz > maxP.pz)
            return false;
    }
    else
    {
        float tMinZ = (minP.pz - rayOrigin.pz) / rayDirection.vz;
        float tMaxZ = (maxP.pz - rayOrigin.pz) / rayDirection.vz;

        if (tMinZ > tMaxZ)
            std::swap(tMinZ, tMaxZ);

        tmin = std::max(tmin, tMinZ);
        tmax = std::min(tmax, tMaxZ);

        if (tmin > tmax)
            return false;
    }

    // ---------------- final hit test ----------------
    if (tmax < 0)
        return false;

    t = tmin;

    // compute hit point
    hitPoint.px = rayOrigin.px + t * rayDirection.vx;
    hitPoint.py = rayOrigin.py + t * rayDirection.vy;
    hitPoint.pz = rayOrigin.pz + t * rayDirection.vz;

    // ---------------- normal calculation ----------------
    float offset = 1e-3;

    if (fabs(hitPoint.px - minP.px) < offset)
        normal.set(-1, 0, 0);
    else if (fabs(hitPoint.px - maxP.px) < offset)
        normal.set(1, 0, 0);
    else if (fabs(hitPoint.py - minP.py) < offset)
        normal.set(0, -1, 0);
    else if (fabs(hitPoint.py - maxP.py) < offset)
        normal.set(0, 1, 0);
    else if (fabs(hitPoint.pz - minP.pz) < offset)
        normal.set(0, 0, -1);
    else
        normal.set(0, 0, 1);

    return true;
}

Phong::Phong()
{
    CameraPos.set(0, 0, 0);
    LightColor.set(0, 0, 0);
    LightDir.set(0, 0, 0);
    ObjectColor.set(0, 0, 0);
    Ka = 0;
    Kd = 0;
    Ks = 0;
    Kp = 0;
}

//----------------------------------------------
Phong::~Phong()
{
}

//----------------------------------------------
void Phong::SetCamera(Point3D pos)
{
    CameraPos = pos;
    if (DEBUG)
        cout << CameraPos.print() << endl;
}

//----------------------------------------------
void Phong::SetLight(ColorRGB color, Vector3D dir)
{
    LightColor = color;
    if (DEBUG)
        cout << LightColor.print() << endl;
    LightDir = dir;
    LightDir.normalize();
    if (DEBUG)
        cout << LightDir.print() << endl;
}

//----------------------------------------------
void Phong::SetObject(ColorRGB color, float ka, float kd, float ks, float kp)
{
    ObjectColor = color;
    if (DEBUG)
        cout << ObjectColor.print() << endl;
    Ka = ka;
    Kd = kd;
    Ks = ks;
    Kp = kp;
}

//----------------------------------------------
void Phong::GetShade(Point3D point, Vector3D normal, ColorRGB &color)
{
    // Normalize surface normal
    normal.normalize();

    // Calculate ambient term
    ColorRGB ambient;
    ambient = LightColor;
    ambient.mult(ObjectColor);
    ambient.mult(Ka);
    if (DEBUG)
        cout << "ambient " << ambient.print() << endl;
    color = ambient;

    // Calculate NdotL value
    float NdotL = normal.dot(LightDir);
    if (DEBUG)
        cout << "NdotL " << NdotL << endl;
    if (NdotL > 0)
    {
        // Calculate diffuse term
        ColorRGB diffuse;
        diffuse = LightColor;
        diffuse.mult(ObjectColor);
        diffuse.mult(Kd * NdotL);
        if (DEBUG)
            cout << "diffuse " << diffuse.print() << endl;
        color.add(diffuse);

        // Calculate R vector
        Vector3D R = normal;
        R.mult(2 * NdotL);
        R.sub(LightDir);
        R.normalize();

        // Calculate V vector
        Vector3D V;
        float Vx = CameraPos.px - point.px;
        float Vy = CameraPos.py - point.py;
        float Vz = CameraPos.pz - point.pz;
        V.set(Vx, Vy, Vz);
        V.normalize();

        // Calculate RdotV value
        float RdotV = R.dot(V);
        if (DEBUG)
            cout << "RdotV " << RdotV << endl;
        if (RdotV > 0)
        {
            // Calculate specular term
            RdotV = pow(RdotV, Kp);
            ColorRGB specular;
            specular = LightColor;
            specular.mult(Ks * RdotV);
            if (DEBUG)
                cout << "specular " << specular.print() << endl;
            color.add(specular);
        }
    }
}

//----------------------------------------------
int test_main()
{
    cout << "\nTest point class\n";
    Point3D p1;
    p1.set(1, 2, 3);
    cout << "p1 = ";
    p1.print();
    cout << endl;

    Point3D p2;
    p2.set(3, 4, 5);
    cout << "p2 = ";
    p2.print();
    cout << endl;

    float dist12 = p1.distance(p2);
    float dist21 = p2.distance(p1);
    cout << "dist12 = " << dist12 << endl;
    cout << "dist21 = " << dist21 << endl;

    cout << "\nTest vector class\n";
    Vector3D v1;
    v1.set(2, 1, 0);
    cout << "v1 = ";
    v1.print();
    cout << endl;

    Vector3D v2;
    v2.set(-1, 1, 0);
    cout << "v2 = ";
    v2.print();
    cout << endl;

    v2.normalize();
    cout << "v2 = ";
    v2.print();
    cout << endl;

    float v1dotv1 = v1.dot(v1);
    float v1dotv2 = v1.dot(v2);
    float v2dotv2 = v2.dot(v2);
    cout << "v1dotv1 = " << v1dotv1 << endl;
    cout << "v1dotv2 = " << v1dotv2 << endl;
    cout << "v2dotv2 = " << v2dotv2 << endl;

    cout << "\nTest ray class\n";
    Ray3D r1;
    r1.set(p1, v1);
    cout << "r1(p1,v1) = ";
    r1.print();
    cout << endl;

    Ray3D r2;
    r2.set(p2, p1);
    cout << "r2(p2,p1) = ";
    r2.print();
    cout << endl;

    for (int i = 0; i <= 10; i++)
    {
        cout << "r1(" << i / 10.0 << ") = ";
        r1.get_sample(i / 10.0).print();
        cout << endl;
    }

    cout << "\nTest sphere class\n";
    Sphere3D sphere;
    Point3D center;
    center.set(0, 0, 0);
    sphere.set(center, 2);
    cout << "sphere = ";
    sphere.print();
    cout << endl;

    for (int px = -3; px <= 3; px++)
    {
        Point3D point;
        point.set(px, 0, 0);
        Vector3D dir;
        dir.set(1, 0, 0);
        Ray3D ray;
        ray.set(point, dir);
        cout << "ray = ";
        ray.print();
        cout << endl;

        Point3D p;
        Vector3D n;
        float t;
        if (!sphere.get_intersection(ray, t, p, n))
            cout << "no intersection\n";
        else
        {
            cout << "point = ";
            p.print();
            cout << endl;
            cout << "normal = ";
            n.print();
            cout << endl;
        }
    }

    cout << "\nTest phong class\n";
    Phong shader;
    string name;
    while (cin >> name)
    {
        if (name == "Camera")
        {
            // Read and save camera information
            Point3D pos;
            cin >> pos.px >> pos.py >> pos.pz;
            shader.SetCamera(pos);
        }

        else if (name == "Light")
        {
            // Read and save light information
            ColorRGB color;
            Vector3D dir;
            cin >> color.R >> color.G >> color.B;
            cin >> dir.vx >> dir.vy >> dir.vz;
            shader.SetLight(color, dir);
        }

        else if (name == "Object")
        {
            // Read and save object information
            ColorRGB color;
            float ka, kd, ks, kp;
            cin >> color.R >> color.G >> color.B;
            cin >> ka >> kd >> ks >> kp;
            shader.SetObject(color, ka, kd, ks, kp);
        }

        else if (name == "Point")
        {
            // Read and use point information
            Point3D point;
            Vector3D normal;
            ColorRGB color;
            cin >> point.px >> point.py >> point.pz;
            cin >> normal.vx >> normal.vy >> normal.vz;
            shader.GetShade(point, normal, color);
            point.print();
            cout << " ";
            color.print();
            cout << endl;
        }

        else
        {
            cout << "Ignoring '" << name << "' token\n";
        }
    }

    return 0;
}
