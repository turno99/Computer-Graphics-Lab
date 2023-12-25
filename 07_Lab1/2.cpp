/*
     Slope Independent Lines
*/
 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <bits/stdc++.h>
using namespace std; 
 
static int slices = 16;
static int stacks = 16;
#define LINE_COUNT 361
#define Width 512
#define Height 512
int lineCoords[LINE_COUNT][4];
 
int generateRandX() {
    return (rand() % Width) - Width/2;
}
 
int generateRandY() {
    return (rand() % Height) - Height/2;
}


void printLines()
{
    for(int i = 0; i<LINE_COUNT; i++)
    {
        printf("For line %d:\n",i);
        printf("Starting point: %d %d\n",lineCoords[i][0],lineCoords[i][1]);
        printf("Ending point: %d %d\n",lineCoords[i][2],lineCoords[i][3]);
    } 
}
 
void generateLines() {
    srand(time(0));
    for (int i = 0; i < LINE_COUNT; i++) {
        lineCoords[i][0] = generateRandX();
        lineCoords[i][1] = generateRandY();
        lineCoords[i][2] = generateRandX();
        lineCoords[i][3] = generateRandY();
    }
    //printLines();
}
 
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Width/2, Width/2 - 1, -Height/2, Height/2 - 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
 
int findZone (int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0;
    if (dx >= 0 and dy >= 0)
    {
        if (dx >= dy)
        {
            //printf("GREEN\n");
            return 0;
        }
        else
        {   //printf("SKYBLUE\n");
            return 1;
        }
    }
    else if (dx < 0 and dy >= 0)
    {
        if (-dx >= dy)
        {
            //printf("YELLOW\n");
            return 3;
        }
        else
        {
           //printf("PURPLE\n");
            return 2;
        }
    }
    else if (dx < 0 and dy < 0)
    {
        if (-dx >= -dy)
        {
             //printf("RED\n");
             return 4;
        }
        else
        {
             //printf("BLUE\n");
             return 5;
        }
    }
    else
    {
        if (dx >= -dy)
        {
           //printf("PINK\n"); 
           return 7;
        }
        else
        {
            //printf("WHITE\n");
             return 6;
        }
    }
}
 
void drawPixel (int x, int y, int zone)
{
    switch (zone)
    {
    case 0:
        glColor3d(0, 102, 102);
        glVertex2i(x, y);
        break;
    case 1:
        glColor3d(0, 102, 0);
        glVertex2i(y, x);
        break;
    case 2:
        glColor3d(255, 255, 0); 
        glVertex2i(-y, x);
        break;
    case 3:
        glColor3f(0.5f, 0.0f, 1.0f);
        glVertex2i(-x, y);
        break;
    case 4:
        glColor3d(0, 0, 255);
        glVertex2i(-x, -y);
        break;
    case 5:
        glColor3d(204, 0, 0);
 
        glVertex2i(-y, -x);
        break;
    case 6:
        glColor3d(102, 0, 51);
 
        glVertex2i(y, -x);
        break;
    case 7:
        glColor3d(255, 255, 255);
 
        glVertex2i(x, -y);
        break;
    }
}
 
void drawLine0 (int x0, int y0, int x1, int y1, int zone)
{
    int dx = x1 - x0, dy = y1 - y0;
    int x = x0, y = y0, d = 2 * dy - dx;
    int de = 2 * dy, dne = 2 * (dy - dx);
    while (x <= x1)
    {
        drawPixel (x, y, zone);
        if (d < 0)
        {
            ++x;
            d += de;
        }
        else
        {
            ++x, ++y;
            d += dne;
        }
    }
}
 
void drawLine (int x0, int y0, int x1, int y1)
{
    int zone = findZone(x0, y0, x1, y1);
    // printf("zone = %d\n", zone);
    switch (zone)
    {
    case 0:
 
        drawLine0(x0, y0, x1, y1, zone);
        break;
    case 1:
 
        drawLine0(y0, x0, y1, x1, zone);
        break;
    case 2:
 
        drawLine0(y0, -x0, y1, -x1, zone);
        break;
    case 3:
 
        drawLine0(-x0, y0, -x1, y1, zone);
        break;
    case 4:
 
        drawLine0(-x0, -y0, -x1, -y1, zone);
        break;
    case 5:
 
        drawLine0(-y0, -x0, -y1, -x1, zone);
        break;
    case 6:
 
        drawLine0(-y0, x0, -y1, x1, zone);
        break;
    case 7:
 
        drawLine0(x0, -y0, x1, -y1, zone);
        break;
    }
}
 
 
int x_0, x_1, y_0, y_1;
 
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-Width/2, 0);
    glVertex2i(Width/2 - 1, 0);
    glVertex2i(0, -Height/2);
    glVertex2i(0, Height/2 - 1);
    glEnd();
 
    glPointSize(2);
    glBegin(GL_POINTS);
    for (int i = 0; i < LINE_COUNT; i++) {
        glColor3d(0,0,1);
        glVertex2i(lineCoords[i][0], lineCoords[i][1]);
        glColor3d(1,0,0);
        glVertex2i(lineCoords[i][2], lineCoords[i][3]);
        glColor3d(1,1,1);
        drawLine(lineCoords[i][0], lineCoords[i][1], lineCoords[i][2], lineCoords[i][3]);
    }
 
    glEnd();
 
    //highlighting end points
    /*glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < LINE_COUNT; i++) {
        glColor3d(0,0,1);
        glVertex2i(lineCoords[i][0], lineCoords[i][1]);
        glColor3d(1,0,0);
        glVertex2i(lineCoords[i][2], lineCoords[i][3]);
        glColor3d(1,1,1);
    }*/
    glEnd();
 
    glutSwapBuffers();
}
 
 
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
 
    case '+':
        slices++;
        stacks++;
        break;
 
    case '-':
        if (slices>3 && stacks>3)
        {
            slices--;
            stacks--;
        }
        break;
    }
 
    glutPostRedisplay();
}
 
static void idle(void)
{
    glutPostRedisplay();
}

void generateCircleSpokes()
{
    double radius = 200.00;
    int step = 1;
    for(int i = 0; i<=360; i+=step)
    {
        double x = radius * cos(i);
        double y = radius * sin(i);
        lineCoords[i][0] = 0;
        lineCoords[i][1] = 0;
        lineCoords[i][2] = x;
        lineCoords[i][3] = y;
    }
    printLines();
} 
// Program entry point
 
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(Width,Height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //generateLines();
    generateCircleSpokes();
    glutCreateWindow("Lab 2: Slope Independent Lines");
       
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
 
 
