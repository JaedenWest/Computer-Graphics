//Program: Homework2 PacMan
//Purpose: implement a program that has pac man fly around and eat space food while being chased by ghosts
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


//CONSTANT
float MIN_SPEED = 0.0005f;
const float SPAWN_BORDER = 0.1f;

void init()
{
    //clear the color
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //set the matrix mode
    glMatrixMode(GL_PROJECTION);
    //specify the camera parameters and viewing volume
    glLoadIdentity();
    //load identity and glortho
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}
//for color 0-1
float randomNumber()
{
    return(float)rand() / RAND_MAX;
}
//for vector points 
float randomCoord()
{
    return randomNumber() * (2.0f - 2.0f * SPAWN_BORDER) - (1.0f - SPAWN_BORDER);
}

class PacMan
{
    public: 
        float x, y;
        float r, g, b;
        float speed;
        float radius;
        float mouthAngle;
        float targetX;
        float targetY;
        int lives;
        float glX;
        float glY;
        float faceDir;

        //constructor
        PacMan()
        {
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            speed = MIN_SPEED + 0.00001;
            radius = 0.1f;
            mouthAngle = 45.0f;
            lives = 3;
        }

        void setColor(float red, float green, float blue)
        {
            r = red;
            g = green;
            b = blue;
        }
                        //mouse x, mouse y
        void setTarget(float mx, float my)
        {
            targetX = mx;
            targetY = my;
        }

        void movement()
        {
            pacmanRotate();
            if(fabs(x - glX) > speed)
            {
                if(x < glX)
                {
                    x += speed;
                }
                else
                {
                    x -= speed;
                }
            }
            else
            {
                x = glX;
            }
            if(fabs(y - glY) > speed)
            {
                if(y < glY)
                { 
                    y += speed;
                }
                else
                {
                    y -= speed;
                }    
            }
            else
            {
                y = glY;
            }           
        }
        void pacmanRotate()
        {
            float dX = glX - x;
            float dY = glY - y;
            faceDir = atan2(dY, dX);
        };
        //it needs to be if (pacmans x is less then the mousepoint, increment x
        //if pacmans y is less than mousepoint increment y
        // and opposites
        // if pacman x or pacman y = mousepoint return)}

        void mouse(int button, int state, int mouseX, int mouseY)
        {
            //while pacman does not equal the point incrementally redraw him a little closer to the point
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                int width = glutGet(GLUT_WINDOW_WIDTH);
                int height = glutGet(GLUT_WINDOW_HEIGHT);
                //converting from pixel coords to gl coords
                glX = (float)mouseX / width * 2.0f - 1.0f;
                glY = -1 * ((float)mouseY / height * 2.0f -1.0f);
                setTarget(glX, glY);
                glutPostRedisplay();
            }   
            
        }
        void draw() const
        {

            glColor3f(r, g, b);
            glBegin(GL_TRIANGLE_FAN);

            //center of the circle
            glVertex2f(x, y);

            int segments = 40;
            float startAngle = mouthAngle * M_PI / 180.0f;
            float endAngle = (360 - mouthAngle) * M_PI / 180.0f;

            for(int i = 0; i <= segments; i++)
            {
                float theta = startAngle + (endAngle - startAngle) * i / segments;
                //the theta + faceDir allows mouth opening to rotate where its moving
                float px = x + radius * cos(theta + faceDir);
                float py = y + radius * sin(theta + faceDir);
                glVertex2f(px, py);
            }


            glEnd();
        }
};


class Pellets
{
    public: 
        float r, g, b;
        float size;
        float Px, Py;
        float Vx, Vy;
        float speed;
        //constructor
        Pellets()
        {
            float theta = randomNumber() * 2.0f * M_PI;
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            size = 0.03f;
            Px = randomCoord();
            Py = randomCoord();
            Vx = cos(theta);
            Vy = sin(theta);
            speed = MIN_SPEED;
        }

        void setColor(float red, float green, float blue)
        {
            r = red;
            g = green;
            b = blue;
        }

        void movePellets()
        {
           //randomly move pellets
            Px += Vx * speed; 
            Py += Vy * speed;

            //bouncing off edges
            if(Px + size >= 1.0f || Px - size <= -1.0f) Vx *= -1;
            if(Py + size >= 1.0f || Py - size <= -1.0f) Vy *= -1;
        }

        void draw()
        {
            glColor3f(r, g, b);
                //a certain shape offset randomly
            glBegin(mode);
                glVertex2f(Px - size , Py - size);
                glVertex2f(Px + size, Py - size);
                glVertex2f(Px + size, Py + size);
                glVertex2f(Px - size, Py + size);
            glEnd();
        }
};


class Ghost
{
    public: 
        float r, g, b;
        float size;
        float Px, Py;
        float Vx, Vy;
        float speed;
        //constructor
        Ghost()
        {
            float theta = randomNumber() * 2.0f * M_PI;
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            Px = randomCoord();
            Py = randomCoord();
            size = 1.0f;
            Vx = cos(theta);
            Vy = sin(theta);
            speed = MIN_SPEED + 0.00002;
        }

        void setColor(float red, float green, float blue)
        {
            r = red;
            g = green;
            b = blue;
        }

        void moveGhosts()
        {
            float halfWidth = 0.05f * size;
            float halfHeight = 0.07f * size;
           //randomly move Ghosts
            Px += Vx * speed; 
            Py += Vy * speed;

            //bouncing off edges
            if(Px + halfWidth > 1.0f || Px - halfWidth < -1.0f) Vx *= -1;
            if(Py + halfHeight > 1.0f || Py - halfHeight < -1.0f) Vy *= -1;
        }

        void draw()
        {
            glColor3f(r, g, b);
            glBegin(GL_LINE_LOOP);

            //left side
            glVertex2f(Px - 0.05f * size, Py - 0.05f * size);

            //bottom waves
            glVertex2f(Px - 0.04f * size, Py - 0.07f * size);  // left edge
            glVertex2f(Px - 0.026f * size, Py - 0.05f * size); // 1st spike tip
            glVertex2f(Px - 0.012f * size, Py - 0.07f * size); // valley
            glVertex2f(Px + 0.0f * size,   Py - 0.05f * size); // 2nd spike tip (center)
            glVertex2f(Px + 0.012f * size, Py - 0.07f * size); // valley
            glVertex2f(Px + 0.026f * size, Py - 0.05f * size); // 3rd spike tip
            glVertex2f(Px + 0.04f * size,  Py - 0.07f * size); // right edge

;
            //right side
            glVertex2f(Px + 0.05f * size, Py - 0.05f * size);

            //top curve
            glVertex2f(Px + 0.05f * size, Py + 0.02f * size);
            glVertex2f(Px + 0.03f * size, Py + 0.03f * size);
            glVertex2f(Px + 0.00f * size, Py + 0.04f * size);
            glVertex2f(Px - 0.03f * size, Py + 0.03f * size);
            glVertex2f(Px - 0.05f * size, Py + 0.02f * size);

            glEnd();
        }
};

bool checkGhostCollision(const PacMan& p, const Ghost& gh)
{
    //comparing the size of pacman with size of ghost
    float dx = p.x - gh.Px;
    float dy = p.y - gh.Py;
    //collision radius is full size of pacman plus full size of ghost
    float collisionRadius = p.radius + 0.07f * gh.size;
    
    return (dx * dx + dy * dy <= collisionRadius * collisionRadius);
};
bool checkPelletCollision(const PacMan& p, const Pellets& pel)
{
        //comparing the edges since pacman is circular
        float dx = p.x - pel.Px;
        float dy = p.y - pel.Py;
        //collision radius is the full size of pacman plus full size of pellet
        float collisionRadius = p.radius + pel.size;
    return(dx * dx + dy * dy <= collisionRadius * collisionRadius);
};


struct GameState
{
    int score;
    int lives;
    bool gameOver;
};

//initializing vector of polygons
//vector<PacMan> pacman;
vector<Pellets> pellets;
vector<Ghost> ghost;
PacMan pacman;
GameState game;
bool printed = false;
void mydisplay()
{
    //clear display buffer and clear the depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draw first polygon
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        
        for(size_t i = 0; i < ghost.size(); i++)
        {
            ghost[i].draw();
        }
        for(size_t i = 0; i < pellets.size(); i++)
        {
            pellets[i].draw();
        }
            pacman.draw();
        printed = true;
    glFlush();
}


//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
    // Check game over
    if ((pacman.lives <= 0) || (game.score >= 50))
    {
        game.gameOver = true;
        return;
    }
    //randomly moving each ghost and pellet
    for(size_t i = 0; i < pellets.size(); i++)
    {
        pellets[i].movePellets();
        if(checkPelletCollision(pacman, pellets[i]))
        {
            if(!pacman.x == 0 && !pacman.y == 0){
                pellets.erase(pellets.begin() + i);
                i--;
                game.score += 10;
                cout << "Score: " << game.score << endl;
                if(game.score == 50)
                {
                    cout << "Game Over" << endl;
                }
            }
        }
    }
    for(size_t i = 0; i < ghost.size(); i++)
    {
        ghost[i].moveGhosts();

        if(checkGhostCollision(pacman, ghost[i]))
        {
            pacman.glX = 0;
            pacman.glY = 0;
            if(!pacman.x == 0 && !pacman.y == 0){
                pacman.x = 0;
                pacman.y = 0;
                pacman.lives--; 
                cout << "Pacman Lives: " << pacman.lives << endl;    
                if(pacman.lives == 0)
                {
                    cout << "Game Over" << endl;
                }   
            }    
        }
    }
    pacman.movement();

        glutPostRedisplay();
}


//int argc is argument counter(total) and argv is vector(breaks each one down like an array ex.1,2,3,4...)  
int main(int argc, char *argv[])
{
//seed for a random number
srand(time(NULL));  
game.score = 0;
game.gameOver = false;
int minGhosts = 3;
int maxGhosts = 5;
int minPellets = 15;
int maxPellets = 20;

//rules of game
cout << "If score = 50 or lives = 0 zero the game will end" << endl;
cout << "Use left click of mouse to move pacman" << endl;

//random number of polygons in min and max
int numGhosts = minGhosts + (rand() % (maxGhosts - minGhosts));
int numPellets = minPellets + (rand() % (maxPellets - minPellets));

pacman.setColor(1, 1, 0);
pacman.x = 0.0f;
pacman.y = 0.0f;

for(int i = 0; i < numGhosts; i++)
{
    Ghost shape;

    //random color
    shape.setColor(randomNumber(), randomNumber(), randomNumber());

    //store it in the vector
    ghost.push_back(shape);

}
for(int i = 0; i < numPellets; i++)
{
    Pellets setShape;

    //random color
    setShape.setColor(1, 1, 0);

    //store it in the vector
    pellets.push_back(setShape);

}


//glutinit
glutInit(&argc, argv);
//specify window size
glutInitWindowSize(750, 750);
//specify window position
glutInitWindowPosition(2000, 0);
//specify the type of display mode where going to use
glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//create the window of whatever shape
glutCreateWindow("PacMan");
//display the function
glutDisplayFunc(mydisplay);
//register mouse callback for pacman
glutMouseFunc([](int button, int state, int x, int y){
    pacman.mouse(button, state, x, y);
});
glutIdleFunc(idle);
//init
init();
//start the event handling loop
glutMainLoop();
//return 0
return 0;

}