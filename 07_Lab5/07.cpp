#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
 
 
int WIDTH = 800, HEIGHT = 600;
int MAX_HEIGHT = HEIGHT/2-1;
int MIN_HEIGHT = -HEIGHT/2;
int MAX_WIDTH = WIDTH/2-1;
int MIN_WIDTH = -WIDTH/2;
float D = 300, L = 400, A = 200;
 
 
struct Point3D {

    int x, y, z;
    Point3D(int xVal = 0, int yVal = 0, int zVal = 0) {
        x = xVal;
        y = yVal;
        z = zVal;
    }
    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }
    void setZ(int zVal) { z = zVal; }
    double distanceTo(const Point3D& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        int dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};
Point3D p2D[8], p3D[8];
// Utility functions
float max(float a, float b){
    return a >= b ? a : b;
}
 
float min(float a, float b){
    return a < b ? a : b;
}
 
 
 
// Line Drawing algorithms zone finding for given line endpoints
int findZone (int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0;
    if (dx >= 0 and dy >= 0)
    {
        if (dx >= dy)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if (dx < 0 and dy >= 0)
    {
        if (-dx >= dy)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else if (dx < 0 and dy < 0)
    {
        if (-dx >= -dy)
        {
             return 4;
        }
        else
        {
             return 5;
        }
    }
    else
    {
        if (dx >= -dy)
        {
           return 7;
        }
        else
        {
             return 6;
        }
    }
}
 
// Drawing pixel with specific color
// The zone argument is used to do zone-0 to any zone conversion
void drawPixel (int x, int y, int zone, int color)
{
    if(color == 0){
        glColor3f(0.0, 1.0, 0.0);
    }
    if(color == 1){
        glColor3f(1.0, 0.0, 0.0);
    }
    if(color == 2){
        glColor3f(1.0, 0.0, 0.0);
    }
    switch (zone)
    {
    case 0:
        glVertex2i(x, y);
        break;
    case 1:
        glVertex2i(y, x);
        break;
    case 2:
        glVertex2i(-y, x);
        break;
    case 3:
        glVertex2i(-x, y);
        break;
    case 4:
        glVertex2i(-x, -y);
        break;
    case 5:
        glVertex2i(-y, -x);
        break;
    case 6:
        glVertex2i(y, -x);
        break;
    case 7:
        glVertex2i(x, -y);
        break;
    }
}
 
// DrawLine function for drawing a line from zone-0
void drawLine0 (int x0, int y0, int x1, int y1, int zone, int color)
{
    int dx = x1 - x0, dy = y1 - y0;
    int x = x0, y = y0, d = 2 * dy - dx;
    int de = 2 * dy, dne = 2 * (dy - dx);
    while (x <= x1)
    {
        drawPixel (x, y, zone, color);
        if (d < 0)
        {
            x++;
            d += de;
        }
        else
        {
            x++;
            y++;
            d += dne;
        }
    }
}
 
 
/*
    Zone Conversion Table
    Zone        Any-To-Zero
    0           (x,y) -> (x,y)
    1           (x,y) -> (y,x)
    2           (x,y) -> (y,-x)
    3           (x,y) -> (-x,y)
    4           (x,y) -> (-x,-y)
    5           (x,y) -> (-y,-x)
    6           (x,y) -> (-y,x)
    7           (x,y) -> (x,-y)
*/
// Finds zone, converts all to zone-0
//      processes in zone-0 but draws in original zone
void draw8way (int x0, int y0, int x1, int y1, int color = 1)
{
    int zone = findZone(x0, y0, x1, y1);
    switch (zone)
    {
    case 0:
        drawLine0(x0, y0, x1, y1, zone, color);
        break;
    case 1:
        drawLine0(y0, x0, y1, x1, zone, color);
        break;
    case 2:
        drawLine0(y0, -x0, y1, -x1, zone, color);
        break;
    case 3:
        drawLine0(-x0, y0, -x1, y1, zone, color);
        break;
    case 4:
        drawLine0(-x0, -y0, -x1, -y1, zone, color);
        break;
    case 5:
 
        drawLine0(-y0, -x0, -y1, -x1, zone, color);
        break;
    case 6:
 
        drawLine0(-y0, x0, -y1, x1, zone, color);
        break;
    case 7:
        drawLine0(x0, -y0, x1, -y1, zone, color);
        break;
    }
}
 
 
 
 
void reshape (int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (-WIDTH/2, WIDTH/2-1, -HEIGHT/2, HEIGHT/2-1);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}
 
 
 
 
//void mouse(int button, int state, int x, int y) {
//    // Save the left button state
//    if (button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN&&!inputDone) {
//        Point p(x + MIN_WIDTH, -y + MAX_HEIGHT);
////        points.push_back(p);
////        counts.push_back(0);
//        print("%d %d\n", p.x, p.y);
//        glutPostRedisplay();
//    }
//}
 
void fillP3d(){
    p3D[0] = Point3D(-A, -A, A);
    p3D[1] = Point3D(A, -A, A);
    p3D[2] = Point3D(A, A, A);
    p3D[3] = Point3D(-A, A, A);
    p3D[4] = Point3D(-A, -A, -A);
    p3D[5] = Point3D(A, -A, -A);
    p3D[6] = Point3D(A, A, -A);
    p3D[7] = Point3D(-A, A, -A);
}
 
void fillP2d(int l, int d){
    p2D[0] = Point3D(-A, -A, A -l -d);
    p2D[1] = Point3D(A, -A, A-l -d);
    p2D[2] = Point3D(A, A, A-l -d);
    p2D[3] = Point3D(-A, A, A-l -d);
    p2D[4] = Point3D(-A, -A, -A-l -d);
    p2D[5] = Point3D(A, -A, -A-l -d);
    p2D[6] = Point3D(A, A, -A-l -d);
    p2D[7] = Point3D(-A, A, -A-l -d);
}
 
void calculateProjectionMatrix(float l, float d){
    for(int i = 0; i < 8; i++){
        Point3D point = p2D[i];
        int x = point.x, y = point.y, z = point.z;
        int xp, yp, zp;
        xp = x/(-z/d);
        yp = y/(-z/d);
        zp = -d;
        p2D[i] = Point3D(xp, yp, zp);
    }
}
 
 
void drawCube(){
    fillP2d(L, D);
    calculateProjectionMatrix(L, D);
    draw8way(p2D[0].x, p2D[0].y, p2D[1].x, p2D[1].y);
    draw8way(p2D[1].x, p2D[1].y, p2D[2].x, p2D[2].y);
    draw8way(p2D[2].x, p2D[2].y, p2D[3].x, p2D[3].y);
    draw8way(p2D[3].x, p2D[3].y, p2D[0].x, p2D[0].y);
    draw8way(p2D[4].x, p2D[4].y, p2D[5].x, p2D[5].y);
    draw8way(p2D[5].x, p2D[5].y, p2D[6].x, p2D[6].y);
    draw8way(p2D[6].x, p2D[6].y, p2D[7].x, p2D[7].y);
    draw8way(p2D[7].x, p2D[7].y, p2D[4].x, p2D[4].y);
    draw8way(p2D[3].x, p2D[3].y, p2D[7].x, p2D[7].y);
    draw8way(p2D[0].x, p2D[0].y, p2D[4].x, p2D[4].y);
    draw8way(p2D[2].x, p2D[2].y, p2D[6].x, p2D[6].y);
    draw8way(p2D[1].x, p2D[1].y, p2D[5].x, p2D[5].y);
}
  
 
void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(MIN_WIDTH, 0);
    glVertex2i(MAX_WIDTH, 0);
    glVertex2i(0, MIN_HEIGHT);
    glVertex2i(0, MAX_HEIGHT);
    glEnd();
    glBegin(GL_POINTS);
    drawCube();
    glEnd();
    glutSwapBuffers();
    glFlush();

}
 
 
void handleKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:
            D+=10.0;
            printf("RIGHT\n");
            break;
        case GLUT_KEY_LEFT:
            D-=10.0;
            printf("LEFT\n");
            break;
        case GLUT_KEY_UP:
            L+=10.0;
            printf("UP\n");
            break;
        case GLUT_KEY_DOWN:
            L-=10.0;
            printf("DOWN\n");
            break;
        default:
            break;
    }
} 

static void idle(void)
{
    glutPostRedisplay();
}
 
int main (int argc, char **argv){
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Transformation Matrix");
    fillP3d();
 
 
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
//    glutKeyboardUpFunc(keyUp);
//    glutKeyboardDownFunc(keyDown);
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutSpecialFunc(handleKeys);
    glutMainLoop();
    return 0;
}
 