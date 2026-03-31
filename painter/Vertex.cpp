//-----------------------------------------------------------
//  Purpose:    Implementation of Vertex class.
//  Author:     John Gauch
//-----------------------------------------------------------

#include "Vertex.h"

Vertex::Vertex()
{
   Px = Py = Pz = 0;
   R = G = B = 0;
}

Vertex::~Vertex()
{
}

void Vertex::transform(Matrix m)
{
   // Apply transformation to vertex with 4x4 matrix multiply
   float newPx = Px*m.data[0][0] + Py*m.data[0][1] + Pz*m.data[0][2] + m.data[0][3];
   float newPy = Px*m.data[1][0] + Py*m.data[1][1] + Pz*m.data[1][2] + m.data[2][3];
   float newPz = Px*m.data[2][0] + Py*m.data[2][1] + Pz*m.data[2][2] + m.data[2][3];
   Px = newPx; Py = newPy; Pz = newPz;
}
