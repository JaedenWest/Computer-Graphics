
#include "ray_classes.h"

int main()
{
   // Great simple polygon
   Poly3D poly;
   Point3D point;
   point.set(1,1,1);
   poly.add_point(point);
   point.set(1,0,1);
   poly.add_point(point);
   point.set(0,0,1);
   poly.add_point(point);
   point.set(0,1,1);
   poly.add_point(point);
   poly.print();

   // Check if point is inside or outside polygon
   for (float y = 0.5; y < 1.5; y += 0.1)
   {
      point.set(0.5,y,0.5);
      point.print();
      if (poly.inside(point)) 
         cout << " inside\n";
      else
         cout << " outside\n";
   }
}

