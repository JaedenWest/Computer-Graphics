//-----------------------------------------------------------
//  Purpose:    Implementation of Scene class.
//  Author:     John Gauch
//-----------------------------------------------------------

#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::transform(Matrix m)
{
    // Apply transformation to all polygons
    for (int i=0; i<polygon.size(); i++)
        polygon[i].transform(m);
}

void Scene::depth()
{
    // Calculate depth for all polygons
    for (int i=0; i<polygon.size(); i++)
        polygon[i].depth();
}

void Scene::partition(int low, int high, int & mid)
{
    // Use polygon[high] for pivot value
    Polygon pivot = polygon[high];
    
    // Partition array into two parts
    int left = low;
    int right = high;
    while (left < right)
    {
        // Scan left to right
        while ((left < right) && (polygon[left].minZ < pivot.minZ))
            left++;
        
        // Scan right to left
        while ((left < right) && (polygon[right].minZ >= pivot.minZ))
            right--;
        
        // Swap polygon values
        Polygon temp = polygon[left];
        polygon[left] = polygon[right];
        polygon[right] = temp;
    }
    
    // Swap pivot to mid
    mid = left;
    polygon[high] = polygon[mid];
    polygon[mid] = pivot;
}

void Scene::sort(int low, int high)
{
    int mid = 0;
    if (low < high)
    {
        // Partition polygons based on depth
        partition(low, high, mid);
        
        // Recursive quicksort calls
        sort(low, mid - 1);
        sort(mid + 1, high);
    }
}

void Scene::sort()
{
    sort(0, polygon.size()-1);
}

void Scene::display()
{
    // Display polygons from back to front
    for (int i=polygon.size()-1; i>=0; i--)
        polygon[i].display();
}

