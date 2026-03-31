//-----------------------------------------------------------
//  Purpose:    Header file for Matrix class.
//  Author:     John Gauch
//-----------------------------------------------------------

#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
using namespace std;

//-----------------------------------------------------------
// Define the Matrix class interface 
//-----------------------------------------------------------
class Matrix
{
 public:
   // Constructors
   Matrix();
   ~Matrix();

   // Methods
   void clear();
   void identity();
   void rotateX(float angle);
   void rotateY(float angle);
   void rotateZ(float angle);
   void translate(float Dx, float Dy, float Dz);
   void scale(float Sx, float Sy, float Sz);
   void multiply(Matrix a, Matrix b);
   void print();

   // Data
   float data[4][4];
};

#endif
