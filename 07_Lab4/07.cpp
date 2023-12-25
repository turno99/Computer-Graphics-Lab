#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

int Width = 640, Height = 480;

int x_min = -100;
int x_max = 100;
int y_min = -100;
int y_max = 100;

int max_y = Height/2-1 ;
int min_y = -Height/2 ;
int max_x = Width/2-1 ;
int min_x = -Width/2 ;

struct Point 
{
    int x, y;
    Point(int x1, int y1)
    {
        x = x1;
        y = y1;
    }

    Point()
    {

    }
};

void reshape (int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-Width/2, Width/2-1, -Height/2, Height/2-1); // size of projection plane (origin at the center)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawClippingWindow(void)
{
    
    glBegin(GL_LINES);

    glVertex2i(x_min,min_y);
    glVertex2i(x_min,max_y);
    glVertex2i(x_max,min_y);
    glVertex2i(x_max,max_y);
    glVertex2i(min_x,y_max);
    glVertex2i(max_x,y_max);
    glVertex2i(min_x,y_min);
    glVertex2i(max_x,y_min);

    glEnd();
}

Point findT(Point p0,Point p1, float t)
{
    return Point((int)(p0.x+t*(p1.x-p0.x)),(int)(p0.y+t*(p1.y-p0.y)));
}

int getZone(int x_0,int y_0, int x_1, int y_1)
{
    int dx = x_1 - x_0;
    int dy = y_1 -y_0;
    if(dx>=0 && dy>=0)
    {
        if (abs(dx)>=abs(dy))
            return 0;
        else
            return 1;
    }
    else if(dx<0 && dy>=0)
    {
        if (abs(dx)>=abs(dy))
            return 3;
        else
            return 2;
    }
    else if(dx<0 && dy<0)
    {
        if (abs(dx)>=abs(dy))
            return 4;
        else
            return 5;
    }
    else if(dx>=0 && dy<0)
    {
        if (abs(dx)>=abs(dy))
            return 7;
        else
            return 6;
    }

}


void drawPixel(int x, int y, int zone)
{
    switch(zone)
    {
    case 0:
        glVertex2i(x,y);
        break;
    case 1:
        glVertex2i(y,x);
        break; 
    case 2:
        glVertex2i(-y,x);
        break;
    case 3:
        glVertex2i(-x,y);
        break;
    case 4:
        glVertex2i(-x,-y);
        break;
    case 5:
        glVertex2i(-y,-x);
        break;
    case 6:
        glVertex2i(y,-x);
        break;
    case 7:
        glVertex2i(x,-y);
        break;
    default:
        break;                     
    }
}

void drawLine0(int x_0,int y_0, int x_1, int y_1, int zone)
{
    int dx = x_1 - x_0;
    int dy = y_1 - y_0;
    int d = 2*dy - dx;
    int delE = 2*dy;
    int delNE = 2*(dy-dx);
    int x = x_0,y = y_0;
    drawPixel(x,y,zone);

    while(x<x_1)
    {
        if(d<0)
        {
            x++;
            d+=delE;
        }
        else
        {
            x++;
            y++;
            d+=delNE;
        }
        drawPixel(x,y,zone);
    }
}

void drawLine(int x_0,int y_0, int x_1, int y_1,int color)
{
    if(color==0)
    {
        glColor3f(1.0, 0.0, 0.0);
    }
    else if(color==1)
    {
        glColor3f(0.0, 1.0, 0.0);
    }
    else if(color==2)
    {
        glColor3f(0.5, 0.5, 0.5);
    }
    int zone = getZone(x_0,y_0,x_1,y_1);
    if(zone==0)
    {
        drawLine0(x_0,y_0,x_1,y_1, 0);
    }
    else if(zone==1)
    {
        drawLine0(y_0,x_0,y_1,x_1,1);
    }
    else if(zone==2)
    {
        drawLine0(y_0,-x_0,y_1,-x_1,2);
    }
    else if(zone==3)
    {
        drawLine0(-x_0,y_0,-x_1,y_1,3);
    }
    else if(zone==4)
    {
        drawLine0(-x_0,-y_0,-x_1,-y_1,4);
    }
    else if(zone==5)
    {
        drawLine0(-y_0,-x_0,-y_1,-x_1,5);
    }
    else if(zone==6)
    {
        drawLine0(-y_0,x_0,-y_1,x_1,6);
    }
    else if(zone==7)
    {
        drawLine0(x_0,-y_0,x_1,-y_1,7);
    }
}


void cyrusBeckLCA(int x0,int y0,int x1,int y1)
{
    float t,temax = 0.0,tlmin = 1.0;

    //top
    t = -(float)(y0-y_max)/(y1-y0);
    if(y1-y0>=0)
    {
        tlmin = min(tlmin,t);
    } 
    else
    {
        temax = max(temax,t);
    }

    //bottom
    t = -(float)(y0-y_min)/(y1-y0);
    if(-(y1-y0)>=0)
    {
        tlmin = min(tlmin,t);
    } 
    else
    {
        temax = max(temax,t);
    }

    //right
    t = -(float)(x0-x_max)/(x1-x0);
    if((x1-x0)>=0)
    {
        tlmin = min(tlmin,t);
    } 
    else
    {
        temax = max(temax,t);
    }

    //left
    t = -(float)(x0-x_min)/(x1-x0);
    if(-(x1-x0)>=0)
    {
        tlmin = min(tlmin,t);
    } 
    else
    {
        temax = max(temax,t);
    }

    // condition for intersecting with clipping window
    if(tlmin<=1.0 && temax>=0.0 && tlmin>=temax)
    { 
        Point p0 = findT(Point(x0,y0),Point(x1,y1),tlmin);
        Point p1 = findT(Point(x0,y0),Point(x1,y1),temax);
        glEnd();
        glPointSize(5);
        glColor3f(1.0,1.0,1.0);

        glBegin(GL_POINTS);
        glVertex2i(p0.x,p0.y);
        glVertex2i(p1.x,p1.y);
        glVertex2i(x0,y0);
        glVertex2i(x1,y1);
        glEnd();
        glPointSize(1);
        glBegin(GL_POINTS);
        drawLine(p0.x,p0.y,p1.x,p1.y,0);
        drawLine(x0,y0,p1.x,p1.y,2);
        drawLine(p0.x,p0.y,x1,y1,2);
    }
    else
    {
        glEnd();
        glPointSize(5);
        glColor3f(1.0,1.0,1.0);

        glBegin(GL_POINTS);
        glVertex2i(x0,y0);
        glVertex2i(x1,y1);
        glEnd();
        glPointSize(1);
        glBegin(GL_POINTS);
        drawLine(x0,y0,x1,y1,1);
    }

}

int linex0, linex1, liney0, liney1;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);

    drawClippingWindow();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);

    int x0 = linex0;
    int x1 = linex1;
    int y0 = liney0;
    int y1 = liney1;

    cyrusBeckLCA(x0,y0,x1,y1);
    
    glEnd();
    glFlush();
}

int main (int argc, char **argv)
{
    freopen("in.txt","r",stdin);
    cin >> linex0 >> liney0 >> linex1 >> liney1;
    srand(time(NULL));
    glutInit (&argc, argv); 
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize (Width, Height); 
    glutInitWindowPosition (10, 10); 
    glutCreateWindow ("Cyrus-Beck Line Clipping"); 
    glutReshapeFunc(reshape);
    glutDisplayFunc (display);
    glutMainLoop(); 
    return 0;
}