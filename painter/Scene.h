//-----------------------------------------------------------
//  Purpose:    Header file for Scene class.
//  Author:     John Gauch
//-----------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Matrix.h"
#include "Polygon.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

//-----------------------------------------------------------
// Define the Scene class interface 
//-----------------------------------------------------------
class Scene
{
 public:
   // Constructors
   Scene();
   ~Scene();

   // Methods
   void transform(Matrix m);
   void depth();
   void partition(int low, int high, int &mid);
   void sort(int low, int high);
   void sort();
   void display();

   // Data
   vector<Polygon> polygon;
};

#endif
