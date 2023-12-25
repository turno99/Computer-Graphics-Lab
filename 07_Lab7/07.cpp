#include <iostream>
#include <fstream>
#include <cstring>
#include <GL/glut.h>
int WIDTH, HEIGHT;
using namespace std;

#define BI_RGB 0

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)
const int sz = 30000000;
unsigned char imageData[sz];
unsigned char negativeImageData[sz];
double normalizedImageData[sz];
double vData[sz];
double hData[sz];
double sData[sz];
int imageSize;
GLuint LoadTexture(const char* filename)
{
    GLuint texture;
    FILE* file;
 //   unsigned char* imageData;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    int width, height, size, offset;

    file = fopen(filename, "rb");
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        return 0;
    }

    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);

    // Check if the color data is stored in reverse byte order (BGR)
    bool reverseBytes = false;
    if (infoHeader.biBitCount == 24 && infoHeader.biCompression == BI_RGB) {
        reverseBytes = true;
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    size = width * height * 3;
    printf("size %d\n",size);
    imageSize = size;
    WIDTH = width;
    HEIGHT = height;
    // Allocate memory for the pixel data buffer
    //imageData = new unsigned char[size];

    // Seek to the beginning of the pixel data
    offset = fileHeader.bfOffBits;
    fseek(file, offset, SEEK_SET);

    // Read the pixel data into the buffer
    fread(imageData, size, 1, file);

    // If the color data is stored in reverse byte order, swap the order of the color bytes
    if (reverseBytes) {
        for (int i = 0; i < size; i += 3) {
            unsigned char temp = imageData[i];
            imageData[i] = imageData[i+2];
            imageData[i+2] = temp;
        }
    }

    for(int i = 0; i < size; i++)
    {
        negativeImageData[i] = 255 - imageData[i];
    }

    for(int i = 0; i < size; i++)
    {
        normalizedImageData[i] = imageData[i];
        normalizedImageData[i]/=255.0;
        cout << "n " <<normalizedImageData[i] << "\n";
    }

    for(int i = 0; i < size; i+=3)
    {
        double mx = max(normalizedImageData[i],max(normalizedImageData[i+1],normalizedImageData[i+2]));
        double mn = min(normalizedImageData[i],min(normalizedImageData[i+1],normalizedImageData[i+2]));
        double r = normalizedImageData[i];
        double g = normalizedImageData[i+1];
        double b = normalizedImageData[i+2];
        vData[i] = max(normalizedImageData[i],max(normalizedImageData[i+1],normalizedImageData[i+2]));
        //vData[i]*=240.0;
        if(vData[i]==0) sData[i] = 0;
        else sData[i] = (mx-mn)/mn;
        double h;
        if(mx==r)
        {
            h = ((g-b)/(mx-mn)) * 60;
        }
        else if(mx==g)
        {
            h = ((b-r)/(mx-mn)) * 60 + 120;
        }
        else
        {
            h = ((r-g)/(mx-mn)) * 60 + 240;
        }
        hData[i] = h;
    }


    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    //delete[] imageData;

    return texture;
}

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void displayOrig()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    int idx = 0;
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            glColor3ub(imageData[idx],imageData[idx+1],imageData[idx+2]);
            glVertex2i(j,i);
            idx+=3;
        }
    }
    glEnd();

/*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, LoadTexture("Input.bmp"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();*/

    glFlush();
    glutSwapBuffers();
}

void displayNeg()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    int idx = 0;
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            glColor3ub(negativeImageData[idx],negativeImageData[idx+1],negativeImageData[idx+2]);
            glVertex2i(j,i);
            idx+=3;
        }
    }
    glEnd();

/*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, LoadTexture("Input.bmp"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();*/

    glFlush();
    glutSwapBuffers();
}

void displayValue()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    int idx = 0;
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            glColor3f(vData[idx],vData[idx],vData[idx]);
            glVertex2i(j,i);
            idx+=3;
        }
    }
    glEnd();

/*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, LoadTexture("Input.bmp"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();*/

    glFlush();
    glutSwapBuffers();
}

void displaySat()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    int idx = 0;
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            glColor3f(sData[idx],sData[idx],sData[idx]);
            glVertex2i(j,i);
            idx+=3;
        }
    }
    glEnd();

/*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, LoadTexture("Input.bmp"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();*/

    glFlush();
    glutSwapBuffers();
}

void displayHue()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    int idx = 0;
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            glColor3f(hData[idx],hData[idx],hData[idx]);
            glVertex2i(j,i);
            idx+=3;
        }
    }
    glEnd();

/*    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, LoadTexture("Input.bmp"));

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();*/

    glFlush();
    glutSwapBuffers();
}



static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    LoadTexture("Input.bmp");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("BMP Image");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayOrig);



    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BMP Image Negative");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayNeg);


    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("BMP Image V");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayValue);


    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("BMP Image S");
    glutReshapeFunc(resize);
    glutDisplayFunc(displaySat);


    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("BMP Image H");
    glutReshapeFunc(resize);
    glutDisplayFunc(displayHue);

    glutMainLoop();
    return 0;
}
//glOrtho(0, xMax, yMax, 0);
//glColor3ub(H,H,H);