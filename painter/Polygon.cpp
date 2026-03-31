//-----------------------------------------------------------
//  Purpose:    Implementation of Polygon class.
//  Author:     John Gauch
//-----------------------------------------------------------

#include "Polygon.h"

Polygon::Polygon()
{
   minZ = 0;
   maxZ = 0;
}

Polygon::~Polygon()
{
}

void Polygon::transform(Matrix m)
{
   // Apply transformation to all vertices
   for (int i=0; i<vertex.size(); i++)
      vertex[i].transform(m);
}

void Polygon::depth()
{
   // Calculate polygon depth
   minZ = vertex[0].Pz; 
   maxZ = vertex[0].Pz;
   for (int i=1; i<vertex.size(); i++)
   {
      if (minZ > vertex[i].Pz) minZ = vertex[i].Pz;
      if (maxZ < vertex[i].Pz) maxZ = vertex[i].Pz;
   }
}

void Polygon::display()
{
   // Render this polygon using OpenGL
   glBegin(GL_POLYGON);
   for (int i=0; i<vertex.size(); i++)
   {
      glColor3f(vertex[i].R, vertex[i].G, vertex[i].B);
      glVertex3f(vertex[i].Px, vertex[i].Py, vertex[i].Pz);
   }
   glEnd();
}
