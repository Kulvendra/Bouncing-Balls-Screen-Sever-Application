//LIBRARIES INCLUDED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <unistd.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "display.h"
#include "main.h"
#include "balls.h"

#define sides 50

#include<iostream>
using namespace std;

extern Ball b[COUNT];

void init()
{
      // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-width/2, width/2, -height/2, height/2, -length/2, length/2);
   //gluPerspective(35,(double)width/height,3,12);
   glEnable(GL_DEPTH_TEST);

}


void DrawCube()
{

    glMatrixMode(GL_MODELVIEW);
	glBegin(GL_QUADS);
	glColor3f( 1.0f, 1.0f, 1.0f);        
	 glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, (-3)*length/8);   
   	 glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, (-3)*length/8);
   	 glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, (-3)*length/8);
   	 glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, (-3)*length/8); 
       

	glColor3f( 1.0f, 1.0f, 0.0f);
	glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f(3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(3*width/8, 3*height/8, 3*length/8);

	glColor3f( 0.0f, 1.0f, 0.0f);
	glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f(3*width/8, 3*height/8, 3*length/8);
	glVertex3f((-1)*3*width/8, 3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, -3*length/8);

	glColor3f( 1.0f, 0.5f, 0.0f);
	glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f((-1)*3*width/8, 3*height/8, 3*length/8);
	glVertex3f((-1)*3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);

	glColor3f( 1.0f, 0.5f, 0.5f);
	glVertex3f((-1)*3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f(3*width/8, (-1)*3*height/8, 3*length/8);
	glEnd();  


}

void display()
{   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 

   DrawCube();

    for (int i = 0; i < COUNT; i++)
    {
		b[i].draw_ball();
    }
	
	glutSwapBuffers();
}



