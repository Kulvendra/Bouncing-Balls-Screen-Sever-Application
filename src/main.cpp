//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "main.h"
#include "physics.h"
#include "balls.h"
#include "display.h"


//CONSTANTS
#define WIDTH 1000
#define HEIGHT 1000
#define LENGTH 1000
#define SPEED 0.1
#define RADIUS 30

bool f= true;
int t =30;


#ifdef RANDOM
	int rndm=1;
#else
	int rndm=0;
#endif

int width=WIDTH;
int height=HEIGHT;
int length=LENGTH;
float speed=SPEED;
float Radius=RADIUS;

//Objects of class Ball and struct Mouse created
Ball b[COUNT];

void paus(){
if(!f)
	f = true;
else f = false;
}

void inc(){
	t++;
	#undef RADIUS
	#define RADIUS t
	Radius = t;
		for (int i=0;i<COUNT;i++){
		b[i].rad=t;
		
	}
	
}
void dec(){
	t--;
	#undef RADIUS
	#define RADIUS t
	Radius = t;
		for (int i=0;i<COUNT;i++){
		b[i].rad=t;
		
	}
}


//speed increase/decrease flags
int acc[COUNT]={};
int dcc[COUNT]={};
	static float px[COUNT];   
	static float py[COUNT];  
	static float pz[COUNT];
	static float vx[COUNT];  
	static float vy[COUNT];  
	static float vz[COUNT];
	


//temporary variables
int colour[COUNT];
int red[COUNT],blue[COUNT],green[COUNT];
int ctr[COUNT]={};

sem_t mutex_rdcnt, mutex_wrcnt, mutex_3, w, r;

void idle()
{
   glutPostRedisplay();
}

void *myThreadFun(void *j)
{
	int i= (long int) j;


	//static 	Array of ball data shared among the threads

	
	px[i]=b[i].get_px(); 
	py[i]=b[i].get_py(); 
	pz[i]=b[i].get_pz();
	vx[i]=b[i].get_vx(); 
	vy[i]=b[i].get_vy();
	vz[i]=b[i].get_vz();

	static int readcount=0;
	static int writecount=0;
	while (true)
	{ 
		if(f){
	
		ctr[i]++;

		{
			float sp=sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i]);
			
			if(dcc[i]&&sp>=0.05) //minimum speed assumed
			{	
				float delta=0.8;
				dcc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;
			}
			
			if(acc[i]&&sp<=6) //maximum speed assumed
			{
				float delta = 1.25;
				acc[i]=0;
				vx[i]*=delta;
				vy[i]*=delta;
				vz[i]*=delta;

			}
		

			if(rndm&&(ctr[i]%1000==0)) //randomly change the speed of the balls at 1000th iteration in random version
			{
				vx[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
				vy[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
				vz[i]=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
			}
		
			//Update postions
			px[i] += vx[i];
			py[i] += vy[i];
			pz[i] += vz[i];
			b[i].set_py(py[i]); 
			b[i].set_px(px[i]);
			b[i].set_pz(pz[i]);
		
			usleep(2000);
		
			red[i]=b[i].get_r2();
			blue[i]=b[i].get_b2();
			green[i]=b[i].get_g2();
		

				//colour[i] = 0;
    
			float tmp[7];
			
			//check for collision with walls
			float *arr= wall(px[i],py[i],pz[i],vx[i],vy[i],vz[i]);
                tmp[0]=arr[0];
                tmp[1]=arr[1];
                tmp[2]=arr[2];
                tmp[3]=arr[3];
                tmp[4]=arr[4];
		tmp[5]=arr[5];
                tmp[6]=arr[6];

                px[i]=tmp[0];
                py[i]=tmp[1];
		pz[i]=tmp[2];
                vx[i]=tmp[3];
                vy[i]=tmp[4];
		vz[i]=tmp[5]; 
                                	 
                int flag=tmp[6];


			if(i<COUNT-1)
			{               
				for(int k=i+1;k<COUNT;k++)
				{       
					sem_wait(&mutex_3);
					sem_wait(&r);
					sem_wait(&mutex_rdcnt);
					readcount++;
					if (readcount == 1)
						sem_wait(&w);
    			
					sem_post(&mutex_rdcnt);
					sem_post(&r);
					sem_post(&mutex_3);
				 
					// Reading started
					float pxk=px[k]; float pyk=py[k];float pzk=pz[k]; float vxk=vx[k]; float vyk=vy[k]; float vzk=vz[k];
					float pxi=px[i]; float pyi=py[i]; float pzi=pz[i]; float vxi=vx[i]; float vyi=vy[i]; float vzi=vz[i];
					// Reading completed
 				 
					sem_wait(&mutex_rdcnt);
					readcount--;
					if (readcount == 0) 
						sem_post(&w);
					sem_post(&mutex_rdcnt);

					//detect overlapping of two balls i adn k
					if (overlap(pxi,pyi,pzi,pxk,pyk,pzk))
					{

						float step = 0.1;
					
						while (overlap(pxi,pyi,pzi,pxk,pyk,pzk))
						{
							pxi-=step*vxi;
							pyi-=step*vyi;
							pzi-=step*vzi; 
							pxk-=step*vxk;
							pyk-=step*vyk;
							pzk-=step*vzk; 
						}
						float vel[6];
						float *temp= velocities(pxi,pyi,pzi,vxi,vyi,vzi,pxk,pyk,pzk,vxk,vyk,vzk);
							vel[0]=*temp++;
							vel[1]=*temp++;
							vel[2]=*temp++;
							vel[3]=*temp++;
							vel[4]=*temp++;
							vel[5]=*temp; 
						
						sem_wait(&mutex_wrcnt);
						writecount++;
						if (writecount == 1) 
							sem_wait(&r);
						sem_post(&mutex_wrcnt); 
						sem_wait(&w);
 					 
						px[i]=pxi; 
						py[i]=pyi; 
						pz[i]=pzi; 
						px[k]=pxk; 
						py[k]=pyk;  
						pz[k]=pzk;
						vx[i]=vel[0]; 
						vy[i]=vel[1];
						vz[i]=vel[2];
						vx[k]=vel[3]; 
						vy[k]=vel[4];
						vz[k]=vel[5]; 
   					 
						sem_post(&w); 
						sem_wait(&mutex_wrcnt);
						writecount--;
						if (writecount == 0) 
							sem_post(&r);
						sem_post(&mutex_wrcnt);
 					 
						b[i].set_vx(vx[i]);
						b[i].set_vy(vy[i]);
						b[i].set_vz(vz[i]); 					 
					}
				}
			}   
		}
	}
}
}

void keyboard(unsigned char key,int x, int y){
switch(key){
case 113:
	for (int i=0;i<COUNT;i++){
		vx[i]=vx[i]+0.5;
		vy[i]=vy[i]+0.5;
		vz[i]=vz[i]+0.5;
	}
	break;
case 97:
		for (int i=0;i<COUNT;i++){
		vx[i]=vx[i]-0.5;
		vy[i]=vy[i]-0.5;
		vz[i]=vz[i]-0.5;
	}
	break;
case 105:
	inc();
	break;
case 106:
	dec();
	break;
case 112:
	paus();


}
}

 
int main(int argc, char *argv[])
{

	sem_init(&mutex_rdcnt, 0, 1);
	sem_init(&mutex_wrcnt, 0, 1);
	sem_init(&mutex_3, 0, 1);
	sem_init(&w, 0, 1);
	sem_init(&r, 0, 1);
	pthread_t tid;
  
    glutInit (&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(500, 500);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Assignment 1");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.0f, -4.0f, 0.0f,0.0f};
 glEnable(GL_COLOR_MATERIAL);
     
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glTranslatef(0.5,0,0);

    init();
	 
	for(long int j=0;j<COUNT;j++) //Create threads
	{
		b[j].set_id(j);
		pthread_create(&tid, NULL, myThreadFun, (void *)j);

	}
	
    glutDisplayFunc(display);
    glutIdleFunc(idle); 
glutKeyboardFunc(keyboard); 
    glutMainLoop();
    
    return 0;
}
