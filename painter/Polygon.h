//-----------------------------------------------------------
//  Purpose:    Header file for Polygon class.
//  Author:     John Gauch
//-----------------------------------------------------------

#ifndef _POLYGON_H
#define _POLYGON_H

#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Matrix.h"
#include "Vertex.h"
#include <vector>
#include <iostream>
using namespace std;

//-----------------------------------------------------------
// Define the Polygon class interface 
//-----------------------------------------------------------
class Polygon
{
 public:
   // Constructors
   Polygon();
   ~Polygon();

   // Methods
   void transform(Matrix m);
   void depth();
   void display();

   // Data
   vector<Vertex> vertex;
   float minZ;
   float maxZ;
};

#endif
