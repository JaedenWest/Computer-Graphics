//-----------------------------------------------------------
//  Purpose:    Implementation of Matrix class.
//  Author:     John Gauch
//-----------------------------------------------------------

#include "Matrix.h"
#include <cmath>
using namespace std;

Matrix::Matrix()
{
   clear();
}

Matrix::~Matrix()
{
}

void Matrix::clear()
{
   for (int r=0; r<4; r++)
   for (int c=0; c<4; c++)
      data[r][c] = 0;
}

void Matrix::identity()
{
   clear();
   for (int i=0; i<4; i++)
      data[i][i] = 1;
}

void Matrix::rotateX(float angle)
{
   identity();
   angle = angle * M_PI / 180;
   data[1][1] = cos(angle);
   data[1][2] = -sin(angle);
   data[2][1] = sin(angle);
   data[2][2] = cos(angle);
}

void Matrix::rotateY(float angle)
{
   identity();
   angle = angle * M_PI / 180;
   data[0][0] = cos(angle);
   data[2][0] = -sin(angle);
   data[0][2] = sin(angle);
   data[2][2] = cos(angle);
}

void Matrix::rotateZ(float angle)
{
   identity();
   angle = angle * M_PI / 180;
   data[0][0] = cos(angle);
   data[0][1] = -sin(angle);
   data[1][0] = sin(angle);
   data[1][1] = cos(angle);
}

void Matrix::translate(float Dx, float Dy, float Dz)
{
   identity();
   data[0][3] = Dx;
   data[1][3] = Dy;
   data[2][3] = Dz;
}

void Matrix::scale(float Sx, float Sy, float Sz)
{
   identity();
   data[0][0] = Sx;
   data[1][1] = Sy;
   data[2][2] = Sz;
}

void Matrix::multiply(Matrix a, Matrix b)
{
   clear();
   for (int r=0; r<4; r++)
   for (int c=0; c<4; c++)
   for (int i=0; i<4; i++)
      data[r][c] += a.data[r][i] * b.data[i][c];
}

void Matrix::print()
{
   for (int r=0; r<4; r++)
   {
      for (int c=0; c<4; c++)
         cout << data[r][c] << " ";
      cout << endl;
   }
}

int test()
{
   Matrix m1;
   m1.scale(3,2,1);
   m1.rotateX(10);
   cout << "m1\n";
   m1.print();

   Matrix m2;
   m2.translate(1,2,3);
   m2.rotateZ(10);
   cout << "m2\n";
   m2.print();

   Matrix m3;
   m3.multiply(m1, m2);
   cout << "m3\n";
   m3.print();

   Matrix m4;
   m4.multiply(m2, m1);
   cout << "m4\n";
   m4.print();

   return 0;
}
