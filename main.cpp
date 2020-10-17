#include <iostream>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "PerlinNoise.h"

using namespace std;

int width = 64;
int height = 64;
float amp = 12;


float* points = new float[width*height];

float randF(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}



void drawGrid(){
    for(int i =0; i<height-2; i++){
        for(int j=0; j<width-2; j++){

                double kof = 20.0;

                double angle = atan(abs(points[i*width+j]-(points[(i+1)*width+j]+points[i*width+j+1])/2)*sqrt(2));
                double shade = max(cos(angle),0.0);
                //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glBegin(GL_POLYGON);
                glColor3f(shade,shade,shade);
                glLineWidth(1);
                glVertex3f((i/(height*1.0))*kof,points[i*width+j],(j/(width*1.0))*kof);
                glVertex3f(((i+1)/(height*1.0))*kof,points[(i+1)*width+j],(j/(width*1.0))*kof);
                glVertex3f((i/(height*1.0))*kof,points[i*width+j+1],((j+1)/(width*1.0))*kof);
                glEnd();


                angle = atan(abs(points[(i+1)*width+j+1]-(points[(i+1)*width+j]+points[i*width+j+1])/2)*sqrt(2));
                shade = max(cos(angle),0.0);
                //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glBegin(GL_POLYGON);
                glColor3f(shade,shade,shade);
                glLineWidth(1);
                glVertex3f(((i+1)/(height*1.0))*kof,points[(i+1)*width+j+1],((j+1)/(width*1.0))*kof);
                glVertex3f(((i+1)/(height*1.0))*kof,points[(i+1)*width+j],(j/(width*1.0))*kof);
                glVertex3f((i/(height*1.0))*kof,points[i*width+j+1],((j+1)/(width*1.0))*kof);
                glEnd();


                glPopMatrix();

        }
    }

}

void display(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13,0,-45);
    glRotatef(40,1,1,0);

    drawGrid();

    glutSwapBuffers();
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,1);
}

void setPoints(int x, int y, int length, float amplitude){
    if(length < 1) return;

    points[y*width+x] += -amplitude+2*randF()*amplitude;
    points[(y+length)*width+x] = -amplitude+2*randF()*amplitude;
    points[(y+length)*width+x+length] = -amplitude+2*randF()*amplitude;
    points[y*width+x+length] = -amplitude+2*randF()*amplitude;


    setPoints(x,y,length/2,amplitude/1.1);
    setPoints(x+length/2,y,length/2,amplitude/1.1);
    setPoints(x+length/2,y+length/2,length/2,amplitude/1.1);
    setPoints(x,y+length/2,length/2,amplitude/1.1);

}

void perlinPoints(int length, float amplitude){
    PerlinNoise pn;
    for(int i = 0; i < length; ++i) {
		for(int j = 0; j < length; ++j) {
			double x = (double)j/((double)length);
			double y = (double)i/((double)length);

			double n = pn.noise(3 * x, 3 * y, 0.8);

			points[i*length+j] = amplitude * n;
		}
	}
}


int main(int argc, char **argv)
{

    srand (time(NULL));
    for(int i=0; i< width*height; i++){
        points[i] = 0;
    }
    perlinPoints(width,amp);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("");
    glutDisplayFunc(display);
    init();
    glutMainLoop();

    delete points;

    return 0;
}
