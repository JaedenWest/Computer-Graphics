//---------------------------------------
// Program: balls.cpp
// Purpose: Use Phong shading to display bouncing
//          balls within a cubic region.
// Author:  John Gauch
// Date:    December 2008
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Constants
#define ROTATE 1
#define TRANSLATE 2
#define RADIUS 100
#define SPEED 0.1
#define COUNT 30
#define SIZE 20
#define STEP 0.1

// Transformation variables 
int xangle = 0;
int yangle = 0;
int zangle = 0;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;

// Ball variables
float Px[COUNT];
float Py[COUNT];
float Pz[COUNT];
float Vx[COUNT];
float Vy[COUNT];
float Vz[COUNT];
float Radius[COUNT];
float Red[COUNT];
float Green[COUNT];
float Blue[COUNT];
float X[SIZE+1][SIZE+1];
float Y[SIZE+1][SIZE+1];
float Z[SIZE+1][SIZE+1];
float Gravity = 0;
float Bounce = -1;

#include "shading.cpp"

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Initialize balls 
//---------------------------------------
void init_balls()
{
   // Initialize balls with random positions and colors
   int i, j;
   for (i=0; i<COUNT; i++)
   {
      // Calculate random positions
      Px[i] = myrand(-RADIUS/2, RADIUS/2);
      Py[i] = myrand(-RADIUS/2, RADIUS/2);
      Pz[i] = myrand(-RADIUS/2, RADIUS/2);
      Vx[i] = myrand(-SPEED, SPEED);
      Vy[i] = myrand(-SPEED, SPEED);
      Vz[i] = myrand(-SPEED, SPEED);
      Radius[i] = myrand(3, 6);

      // Genterate random colors
      Red[i] = myrand(0, 1);
      Green[i] = myrand(0, 1);
      Blue[i] = myrand(0, 1);
   } 

   // Initialize ball surface
   for (i = 0; i <= SIZE; i++)
   {
      float angle1 = 2 * i * M_PI / SIZE;
      for (j = 0; j <= SIZE; j++)
      {
         float angle2 = j * M_PI / SIZE;
         X[i][j] = cos(angle1) * sin(angle2);
         Y[i][j] = sin(angle1) * sin(angle2);
         Z[i][j] = cos(angle2);
      }
   }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-RADIUS, RADIUS, -RADIUS, RADIUS, -RADIUS, RADIUS);
   glEnable(GL_DEPTH_TEST);

   // Initialize smooth shading
   glShadeModel(GL_SMOOTH);
   init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
   init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1);
   init_light(GL_LIGHT2, 1, 1, 0, 1, 1, 1);

   // Initialize bouncing balls
   init_balls();
}

//---------------------------------------
// Draw 3D cube
//---------------------------------------
void draw_cube(float midx, float midy, float midz, float size)
{
   // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

   // Draw 6 faces
   glBegin(GL_LINE_LOOP);
   glVertex3f(ax, ay, az);
   glVertex3f(bx, by, bz);
   glVertex3f(cx, cy, cz);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3f(ax, ay, az);
   glVertex3f(dx, dy, dz);
   glVertex3f(hx, hy, hz);
   glVertex3f(ex, ey, ez);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3f(ax, ay, az);
   glVertex3f(ex, ey, ez);
   glVertex3f(fx, fy, fz);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3f(gx, gy, gz);
   glVertex3f(fx, fy, fz);
   glVertex3f(ex, ey, ez);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3f(gx, gy, gz);
   glVertex3f(cx, cy, cz);
   glVertex3f(bx, by, bz);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3f(gx, gy, gz);
   glVertex3f(hx, hy, hz);
   glVertex3f(dx, dy, dz);
   glVertex3f(cx, cy, cz);
   glEnd();
}
//---------------------------------------
// Draw 3D ball 
//---------------------------------------
void draw_ball(float centerX, float centerY, 
float centerZ, float radius)
{
   // Draw the surface
   int i, j;
   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
      {
         glBegin(GL_POLYGON);
         glNormal3f(X[i][j], Y[i][j], Z[i][j]);
         glVertex3f(centerX+radius*X[i][j], 
            centerY+radius*Y[i][j], centerZ+radius*Z[i][j]);

         glNormal3f(X[i][j + 1], Y[i][j + 1], Z[i][j + 1]);
         glVertex3f(centerX+radius*X[i][j + 1], 
            centerY+radius*Y[i][j + 1], centerZ+radius*Z[i][j + 1]);

         glNormal3f(X[i + 1][j + 1], Y[i + 1][j + 1], Z[i + 1][j + 1]);
         glVertex3f(centerX+radius*X[i + 1][j + 1], 
            centerY+radius*Y[i + 1][j + 1], centerZ+radius*Z[i + 1][j + 1]);

         glNormal3f(X[i + 1][j], Y[i + 1][j], Z[i + 1][j]);
         glVertex3f(centerX+radius*X[i + 1][j], 
            centerY+radius*Y[i + 1][j], centerZ+radius*Z[i + 1][j]);
         glEnd();
      }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Draw the bouncing balls
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Initialize material properties
      init_material(Ka, Kd, Ks, 100 * Kp, Red[i], Green[i], Blue[i]);

      // Draw ball at current location
      draw_ball(Px[i], Py[i], Pz[i], Radius[i]);
   }

   // Draw bounding box
   init_material(1, 0, 0, 0, 1, 1, 1);
   draw_cube(0,0,0,RADIUS);
   glutSwapBuffers();
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
   // Move bouncing balls
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Update ball position
      Px[i] += Vx[i];
      Py[i] += Vy[i];
      Pz[i] += Vz[i];

      // Update ball velocity
      Vy[i] -= Gravity;
 
      // Bounce off walls
      if (Px[i] > RADIUS/2 - Radius[i]) 
         {Px[i] = RADIUS/2 - Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] > RADIUS/2 - Radius[i]) 
         {Py[i] = RADIUS/2 - Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] > RADIUS/2 - Radius[i]) 
         {Pz[i] = RADIUS/2 - Radius[i]; Vz[i] *= Bounce; }
      if (Px[i] < -RADIUS/2 + Radius[i]) 
         {Px[i] = -RADIUS/2 + Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] < -RADIUS/2 + Radius[i]) 
         {Py[i] = -RADIUS/2 + Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] < -RADIUS/2 + Radius[i]) 
         {Pz[i] = -RADIUS/2 + Radius[i]; Vz[i] *= Bounce; }

   }
   glutPostRedisplay();
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
   // Move bouncing balls
   int i;
   for (i = 0; i < COUNT; i++)
   {
      // Update ball position
      Px[i] += Vx[i];
      Py[i] += Vy[i];
      Pz[i] += Vz[i];

      // Update ball velocity
      Vy[i] -= Gravity;
 
      // Bounce off walls
      if (Px[i] > RADIUS/2 - Radius[i]) 
         {Px[i] = RADIUS/2 - Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] > RADIUS/2 - Radius[i]) 
         {Py[i] = RADIUS/2 - Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] > RADIUS/2 - Radius[i]) 
         {Pz[i] = RADIUS/2 - Radius[i]; Vz[i] *= Bounce; }
      if (Px[i] < -RADIUS/2 + Radius[i]) 
         {Px[i] = -RADIUS/2 + Radius[i]; Vx[i] *= Bounce; }
      if (Py[i] < -RADIUS/2 + Radius[i]) 
         {Py[i] = -RADIUS/2 + Radius[i]; Vy[i] *= Bounce; }
      if (Pz[i] < -RADIUS/2 + Radius[i]) 
         {Pz[i] = -RADIUS/2 + Radius[i]; Vz[i] *= Bounce; }

   }
   glutPostRedisplay();
   glutTimerFunc(5, timer, 0);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Determine if we are in ROTATE or TRANSLATE mode
   if ((key == 'r') || (key == 'R'))
      mode = ROTATE;
   else if ((key == 't') || (key == 'T'))
      mode = TRANSLATE;

   // Update ROTATE parameters
   if (mode == ROTATE)
   {
      if (key == 'x')
	 xangle -= 5;
      else if (key == 'y')
	 yangle -= 5;
      else if (key == 'z')
	 zangle -= 5;
      else if (key == 'X')
	 xangle += 5;
      else if (key == 'Y')
	 yangle += 5;
      else if (key == 'Z')
	 zangle += 5;
   }

   // Update TRANSLATE parameters
   if (mode == TRANSLATE)
   {
      if (key == 'x')
	 xpos -= 5;
      else if (key == 'y')
	 ypos -= 5;
      else if (key == 'z')
	 zpos -= 5;
      else if (key == 'X')
	 xpos += 5;
      else if (key == 'Y')
	 ypos += 5;
      else if (key == 'Z')
	 zpos += 5;
   }

   // Update MATERIAL properties
   if (key == 'a')
      Ka -= STEP;
   if (key == 'd')
      Kd -= STEP;
   if (key == 's')
      Ks -= STEP;
   if (key == 'p')
      Kp -= STEP;
   if (key == 'A')
      Ka += STEP;
   if (key == 'D')
      Kd += STEP;
   if (key == 'S')
      Ks += STEP;
   if (key == 'P')
      Kp += STEP;
   if (Ka < 0)
      Ka = 0;
   if (Kd < 0)
      Kd = 0;
   if (Ks < 0)
      Ks = 0;
   if (Kp < STEP)
      Kp = STEP;

   // Update BALL properties
   int i;
   if (key == '+')
      for (i = 0; i < COUNT; i++)
      {
         Vx[i] *= 1.05;
         Vy[i] *= 1.05;
         Vz[i] *= 1.05;
      }
   if (key == '-')
      for (i = 0; i < COUNT; i++)
      {
         Vx[i] /= 1.05;
         Vy[i] /= 1.05;
         Vz[i] /= 1.05;
      }
   if (key == 'g')
      Gravity = 0;
   if (key == 'G')
      Gravity = 1;
   if (key == 'b')
      Bounce *= 0.9;
   if (key == 'B')
      Bounce /= 0.9;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Print command menu
//---------------------------------------
void print_menu()
{
   printf("\nObject controls:\n");
   printf("  '-' = decrease speed\n");
   printf("  '+' = increase speed\n");
   printf("  'b' = decrease bounce\n");
   printf("  'B' = increase bounce\n");
   printf("  'g' = turn gravity off\n");
   printf("  'G' = turn gravity on\n");
   printf("\nLighting controls:\n");
   printf("  'a' = decrease ambient light\n");
   printf("  'A' = increase ambient light\n");
   printf("  'd' = decrease diffuse light\n");
   printf("  'D' = increase diffuse light\n");
   printf("  's' = decrease specular light\n");
   printf("  'S' = increase specular light\n");
   printf("  'p' = decrease specular light exponent\n");
   printf("  'P' = increase specular light exponent\n");
   printf("\nMotion controls:\n");
   printf("  'r' = switch to rotation mode\n");
   printf("  't' = switch to translation mode\n");
   printf("  'x' = decrease X translation/rotation\n");
   printf("  'X' = increase X translation/rotation\n");
   printf("  'y' = decrease Y translation/rotation\n");
   printf("  'Y' = increase Y translation/rotation\n");
   printf("  'z' = decrease Z translation/rotation\n");
   printf("  'Z' = increase Z translation/rotation\n");
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Handle mouse down
   static int xdown, ydown;
   if (state == GLUT_DOWN)
   {
      xdown = x;
      ydown = y;
   }

   // Handle ROTATE
   if ((mode == ROTATE) && (state == GLUT_UP))
   {
      xangle += (y - ydown);
      yangle -= (x - xdown);
      zangle = 0;
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if ((mode == TRANSLATE) && (state == GLUT_UP))
   {
      xpos += (x - xdown);
      ypos -= (y - ydown);
      glutPostRedisplay();
   }
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Balls");
   init();
   print_menu();

   // Specify callback function
   glutDisplayFunc(display);
   // glutIdleFunc(idle);
   glutTimerFunc(5, timer, 0);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}
