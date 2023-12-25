#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
 
#include<bits/stdc++.h>
using namespace std; 
#define WIDTH 512
#define HEIGHT 512
 
int rad = 50;
double original_a = 0, original_b = 0;
double a = 0, b = 0;
double b_dec = 0.5;
double original_cx = 0, original_cy = HEIGHT / 4;
double cx = 0, cy = 0;
double dx = 0.8, dy = -1.8, g = -4.5;
double prefix_sum_dx = 0, prefix_sum_dy = 0;
double area = 0;
double t = 0;
double t_inc = 0.01;
int falling = 1;
int flag = 0;



static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
 
void drawPixel(double x, double y)
{
    glVertex2d(x, y);
}
 
void draw4way(double x, double y) {
    drawPixel(x + cx, y + cy);
    drawPixel(-x + cx, y + cy);
    drawPixel(-x + cx, -y + cy);
    drawPixel(x + cx, -y + cy);
}
 
void drawRegion1(double a, double b) {
    double x = 0, y = b;
    double d = 4*b*b - 4*a*a*b + a*a;
    draw4way(x, y);
    while ((2*b*b*x + 2*b*b) < (2*a*a*y - a*a)) {
        if (d < 0) {
            d += (2*b*b*x + 3*b*b) * 4; // delE
            x++;
        } else {
            d += (2*b*b*x + 3*b*b - 2*a*a - 2*a*a*y) * 4; // delSE
            x++;
            y--;
        }
        draw4way(x, y);
    }
}
 
void drawRegion2(double a, double b) {
    double x = a, y = 0;
    double d = 4 * a * a - 4 * a * b * b + b * b;
    draw4way(x, y);
    while ((2*b*b*x + 2*b*b) >= (2*a*a*y - a*a)) {
        if (d < 0) {
            d += (2*a*a*y + 3*a*a) * 4; // delE
            y++;
        } else {
            d += (2*a*a*y + 3*a*a - 2*b*b*x + 2*b*b) * 4; // delSE
            x--;
            y++;
        }
        draw4way(x, y);
    }
}
 
void drawEllipse() {
    drawRegion1(a, b);
    drawRegion2(a, b);
}
 
static void display(void)
{
    if (flag) {
        getchar();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(100, 100, 100);
    glBegin(GL_LINES);
    glVertex2i(-WIDTH/2, 0);
    glVertex2i(WIDTH/2, 0);
    glVertex2i(0, -HEIGHT/2);
    glVertex2i(0, HEIGHT/2);
    glEnd();
 
    glPointSize(2);
    glBegin(GL_POINTS);
 
    if (falling && ((cy - b) <= -HEIGHT/2)) {
        dy = 0;
        b -= b_dec;
        if (b <= original_b / 2) b_dec *= -1;
        cy = -HEIGHT/2 + b;
        a = sqrt(area - b * b);
        drawEllipse();
        if (b == original_b) {
            falling = 0;
            g *= -1;
            t_inc *= -1;
            b_dec *= -1;
        }
    }
    else {
        
        drawEllipse();
 
        if ((cy) > original_cy) {
            g *= -1;
            falling = 1;
        };
 
        t += t_inc;
        dy = 0.5*g*t*t;
        prefix_sum_dy += dy;
        cy = original_cy + prefix_sum_dy;
    }
 
    glEnd();
 
    glutSwapBuffers();
    flag = 1;
}
 
static void idle(void)
{
    glutPostRedisplay();
}

 
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Ellipse Drawing");
 
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
 
    cx = 0;
    cy = HEIGHT/4;
 
    original_a = rad;
    original_b = rad;
    a = original_a;
    b = original_b;
    area = original_a * original_a + original_b * original_b;
 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}