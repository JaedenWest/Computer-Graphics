//-----------------------------------------------------------
//  Purpose:    Header file for Vertex class.
//  Author:     John Gauch
//-----------------------------------------------------------

#ifndef _VERTEX_H
#define _VERTEX_H

#include "Matrix.h"
#include <iostream>
using namespace std;

//-----------------------------------------------------------
// Define the Vertex class interface 
//-----------------------------------------------------------
class Vertex
{
 public:
   // Constructors
   Vertex();
   ~Vertex();

   // Methods
   void transform(Matrix m);

   // Data
   float Px, Py, Pz;
   float R, G, B;
};

#endif
