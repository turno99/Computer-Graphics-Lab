/*
    CIRCLE
*/
 
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
 
#include<bits/stdc++.h>
using namespace std; 
 
static int slices = 16;
static int stacks = 16;
#define LINE_COUNT 10
#define Height 512
#define Width 512

int lineCoords[LINE_COUNT][4];
 
int generateRandX() {
    return (rand() % Width) - Width/2;
}
 
int generateRandY() {
    return (rand() % Height) - Height/2;
}
 
void generateLines() {
    for (int i = 0; i < LINE_COUNT; i++) {
        lineCoords[i][0] = generateRandX();
        lineCoords[i][1] = generateRandY();
        lineCoords[i][2] = generateRandX();
        lineCoords[i][3] = generateRandY();
    }
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
            printf("GREEN\n");
            return 0;
        }
        else
        {
            printf("SKYBLUE\n");
            return 1;
        }
    }
    else if (dx < 0 and dy >= 0)
    {
        if (-dx >= dy)
        {
            printf("YELLOW\n");
            return 3;
        }
        else
        {
            printf("PURPLE\n");
            return 2;
        }
    }
    else if (dx < 0 and dy < 0)
    {
        if (-dx >= -dy)
        {
             printf("RED\n");
             return 4;
        }
        else
        {
             printf("BLUE\n");
             return 5;
        }
    }
    else
    {
        if (dx >= -dy)
        {
            printf("PINK\n"); 
            return 7;
        }
        else
        {
             printf("WHITE\n");
             return 6;
        }
    }
}
 
void drawPixel(double x, double y)
{
    glVertex2f(x, y);
}
 
// (a,b) is the centre of the circle
double radius, a, b;  
void draw8way(double x, double y){
    drawPixel(x+a, y+b);
    drawPixel(y+a, x+b);
    drawPixel(-y+a, x+b);
    drawPixel(-x+a, y+b);
    drawPixel(-x+a, -y+b);
    drawPixel(-y+a, -x+b);
    drawPixel(y+a, -x+b);
    drawPixel(x+a, -y+b);
}

struct tableEntry
{
    int x, y, d;
    string decision;
};

vector<tableEntry> table;
 

// rate of change
double dx = 0.2, dy = 0.5; 
void drawCircle1(int r){
    int x = 0, y = r;
    int d = 5- 4*r;
    //tableEntry t;
    
    draw8way(x, y);
    while(x < y){
        //t.x = x;
        //t.y = y;
        //t.d = d;
        if(d < 0){
            d += (2*x + 3) *4;
            x++;
            //t.decision = "delE";
        }else{
            d += (2*x - 2*y + 5) * 4;
            x++;
            y--;
            //t.decision = "delSE";
        }
        //table.push_back(t);
        draw8way(x, y);
    }

    
}
 
double ymax = Height/2, ymin = -Height/2, xmax = Width/2, xmin = -Width/2; 
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
//    for (int i = 0; i < LINE_COUNT; i++) {
//        printf("%d %d %d %d\n", lineCoords[i][0], lineCoords[i][1], lineCoords[i][2], lineCoords[i][3]);
//        glColor3d(0,0,1);
//        glVertex2i(lineCoords[i][0], lineCoords[i][1]);
//        glColor3d(1,0,0);
//        glVertex2i(lineCoords[i][2], lineCoords[i][3]);
//        glColor3d(1,1,1);
//        drawLine(lineCoords[i][0], lineCoords[i][1], lineCoords[i][2], lineCoords[i][3]);
//    }
    drawCircle1(radius);

    //collision detection
    if(a+radius>=xmax) dx*=-1; // collision with right wall
    else if(a-radius<=xmin) dx*=-1; // collision with left wall

    if(b+radius>=ymax) dy*=-1; //collision with top wall
    else if(b-radius<=ymin) dy*=-1; //collision with bottom wall
    a+=dx;
    b+=dy;
    glEnd();
 
//    glPointSize(10);
//    glBegin(GL_POINTS);
//    for (int i = 0; i < LINE_COUNT; i++) {
//        glColor3d(0,0,1);
//        glVertex2i(lineCoords[i][0], lineCoords[i][1]);
//        glColor3d(1,0,0);
//        glVertex2i(lineCoords[i][2], lineCoords[i][3]);
//        glColor3d(1,1,1);
//    }
//    glEnd();
 
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
 
// Program entry point
 
int main(int argc, char *argv[])
{
    printf("Enter Center of Circle in following format: x y\n");
    scanf("%lf %lf",&a,&b);
    printf("Enter Radius of Circle:\n");
    scanf("%lf",&radius);
    glutInit(&argc, argv);
    glutInitWindowSize(Width,Height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    generateLines();
    glutCreateWindow("Circle Drawing");
 
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}
// g++ Circle.cpp -o Circle -lglut -lGLU -lGL