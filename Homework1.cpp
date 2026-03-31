//Program: Homework1 Polygon
//Purpose: implement a c++ program that intializes a vector of P polygons
//Author: Jaeden West
#include <ctime>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#ifdef Mac
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif
using namespace std;
GLenum mode = GL_POLYGON;

void init(){
    //clear the color
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //enable the z or depth of the shape
    glEnable(GL_DEPTH_TEST);
    //set the matrix mode
    glMatrixMode(GL_PROJECTION);
    //specify the camera parameters and viewing volume
    glLoadIdentity();
    //load identity and glortho
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}
//for color 0-1
float randomNumber(){
    return(float)rand() / RAND_MAX;
}
//for vector points
float randomCoord(){
    return randomNumber() * 2.0f - 1.0f;
}

class Polygon3D{
    public: 
        vector<float> x, y, z;
        float r, g, b;
        //constructor
        Polygon3D(){
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
        }

        void setColor(float red, float green, float blue){
            r = red;
            g = green;
            b = blue;
        }

        void setVertex(float vx, float vy, float vz){
            x.push_back(vx);
            y.push_back(vy);
            z.push_back(vz);

        }
        int getN() const{
            return x.size();
        }


        void printData(int polygonNumber){
            cout <<"Polygon:" << polygonNumber << endl;
            cout << "How Many Vertices:" << getN() << endl;
            cout <<"Color:" << endl;
            cout << "r: " << r << endl << "g: " << g << endl<< "b: " << b << endl;
            for(int i = 0; i < getN(); i++){
                cout << "Vertex " << i << ":" 
                << x[i] << " " << y[i] << " " << z[i] << endl;
            }
        }

        void draw(){
            glColor3f(r, g, b);
            glBegin(GL_POLYGON);
            for(int i = 0; i < getN(); i++){
                glVertex3f(x[i], y[i], z[i]);
            }
            glEnd();
        }
};

//initializing vector of polygons
vector<Polygon3D> polygons;
bool printed = false;
float theta = 30.0f;
float full2Rotate = 720.0f;
float half2Rotate = 360.0f;
float counter = 30.0f;
void mydisplay(){
    //clear display buffer and clear the depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draw first polygon
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(!printed){
        for(size_t i = 0; i < polygons.size(); i++){
            polygons[i].printData(i + 1);
        }
        printed = true;
    }

    for(size_t i = 0; i < polygons.size(); i++){
        if(counter < half2Rotate)
        {
            glRotatef(theta, 0.0, 1.0, 0.0);
            theta += 1.0f;
            counter += 1.0f;

        }
        else if(counter < full2Rotate){
            glRotatef(theta, 0.0, 1.0, 0.0);
            theta -= 1.0f;
            counter += 1.0f;
            
        }
        else{
            counter = 30.0f;
           // theta = 30.0f;
        }
            polygons[i].draw();
    }
    cout << endl << "Theta: " << theta << endl;
    //if(getting bigger){
    //rotate it bigger by a theta}
    //else{
    //rotate it smaller by a theta}
    glFlush();
}




//int argc is argument counter(total) and argv is vector(breaks each one down like an array ex.1,2,3,4...)  
int main(int argc, char *argv[]){
//seed for a random number
srand(time(NULL));  

int minPolygons = 5;
int maxPolygons = 10;

//random number of polygons in min and max
int numPolygons = minPolygons + (rand() % (maxPolygons - minPolygons));

//initializing vector of polygons
//vector<Polygon3D> polygons;

for(int i = 0; i < numPolygons; i++){
    Polygon3D shape;

    //random color
    shape.setColor(randomNumber(), randomNumber(), randomNumber());

    //random vertices

    for(int j = 0; j < 4; j++){
        shape.setVertex(randomCoord(), randomCoord(), randomCoord());
    }

    //store it in the vector
    polygons.push_back(shape);

}


//glutinit
glutInit(&argc, argv);
//specify window size
glutInitWindowSize(500, 500);
//specify window position
glutInitWindowPosition(250, 250);
//specify the type of display mode where going to use
glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
//create the window of whatever shape
glutCreateWindow("3D Polygon");
//display the function
glutDisplayFunc(mydisplay);
//init
init();
//start the event handling loop
glutMainLoop();
//return 0
return 0;

}