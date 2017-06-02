#include "stdafx.h"
#include <gl/glut.h>
#include <gl/gl.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <string.h>
#define M_PI 3.14

int about_int=0;								//define glu object
int s=0;										//for bacground color array calculation
const float clockR    = 80.0,					//clock size in comparision to screen ie clock volume
			clockVol  = 100.0,					//clock volume screen comaprision with clockr

            angle1min = M_PI / 30.0,			// Distance angle between two lines

            minStart  = 0.98,					// minute hand starting position
            minEnd    = 1.0,					// minute hand endind position

            hourStart = 0.85,					// hour hand starting position
            hourEnd   = 1.0;					// hour hand ending position

float	angleHour = 0,							// parameter of the hour hand
		angleMin  = 0,							// parameter of the minute hand
		angleSec  = 0;							// parameter of second hand

//draw straight lines such as: hour, minute hand, second hand,Bar subdivision, large graduations
void newLine(float rStart, float rEnd, float angle)
{
	float c = cos(angle), s = sin(angle);			//angle calculation for watch hand
	glVertex2f(clockR*rStart*c, clockR*rStart*s);	//makes the line for the hour hand
	glVertex2f(clockR*rEnd*c, clockR*rEnd*s);		//makes the line for the minute hand
}



void display(void)
{						//draw: Outer circle includes hour, minute hand, second hand
	int i;											//for minute counter
	glClear(GL_COLOR_BUFFER_BIT);					
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(4.0);								//defines the width of the minute and hour indicatior 
	glBegin(GL_LINES);
    for(i=0; i<60; i++)
	{
      if(i%5)
	  {				
        if(i%5 == 1)			
        glColor3f(1.0, 1.0, 1.0);					//defines the color of the minute hand
		newLine(minStart, minEnd, i*angle1min);		//call the function to make minute interval marks(line)
      }
	  else
	  {							
       glColor3f(1.0, 1.0, 1.0);					//defines the color of the hour hand
        newLine(hourStart, hourEnd, i*angle1min);	//call the function to make hour interval marks(line)
      }
    }
	glEnd();

  //code to draw houe hand and minute hand
  glLineWidth(3.0);
  glColor3f(0.7, 0.7, 0.7);
  glBegin(GL_LINES);
    newLine(0.0, 0.5, -angleHour+M_PI/2);			//defines the hour hand
    newLine(0.0, 0.8, -angleMin+M_PI/2);			//defines the minute hand
  glEnd();
  
  //code to draw the second hand
  glLineWidth(1.0);
   glColor3f(0.7, 0.7, 0.7);
  glBegin(GL_LINES);
    newLine(0.0, 0.8, -angleSec+M_PI/2);			//defines the second hand
  glEnd();
  glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h)
{
  GLfloat aspectRatio;

  // Prevent a divide by zero
  if(h == 0)
    h = 1;

  // Set Viewport to window dimensions
  glViewport(0, 0, w, h);

  // Reset coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Establish clipping volume (left, right, bottom, top, near, far)
  aspectRatio = (GLfloat)w / (GLfloat)h;
  if (w <= h)
    glOrtho (-clockVol, clockVol, -clockVol/aspectRatio, clockVol/aspectRatio, 1.0, -1.0);
  else
    glOrtho (-clockVol *aspectRatio, clockVol *aspectRatio, -clockVol, clockVol, 1.0, -1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void TimerFunction(int value)
{
  struct timeb tb;
  time_t tim=time(0);
  struct tm* t;
  t=localtime(&tim);
  ftime(&tb);

  angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0 * M_PI;
  angleMin = (float)(t->tm_min)/30.0 * M_PI + angleSec/60.0;
  angleHour = (float)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0 * M_PI+
              angleMin/12.0;
  glutPostRedisplay();
  glutTimerFunc(33,TimerFunction, 1);
}

void bground(int c)
{
	if(c>7)
	{
		c-=7;
	}
	switch(c)
	{
		case 0: glClearColor(0.0, 0.0, 0.0, 1.0);
				return;
		case 1: glClearColor(0.0, 0.0, 1.0, 1.0);
				return;
		case 2: glClearColor(0.0, 1.0, 0.0, 1.0);
				return;
		case 3: glClearColor(0.0, 1.0, 1.0, 1.0);
				return;
		case 4: glClearColor(1.0, 0.0, 0.0, 1.0);
				return;
		case 5: glClearColor(1.0, 0.0, 1.0, 1.0);
				return;
		case 6: glClearColor(1.0, 1.0, 0.0, 1.0);
				return;
		case 7: glClearColor(1.0, 1.0, 1.0, 1.0);
				return;
		default:return;

	}
	glutPostRedisplay();
}

void mouse(int btm,int state,int x,int y)
{
	if(btm==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		if(s==7)
			s=0;
		else
			s=s+1;
		}
	if(btm==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
	{
		s=0;
	}
	bground(s);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
	{
		case 46:
			{
			if(s==7)
				s=0;
			else
				s=s+1;
			bground(s);	
			break;
			}
		case 44:
			{
			if(s==0)
				s=7;
			else
				s=s-1;
			bground(s);
			break;
			}
		case 27:
            exit(0);
	}
}

void mainmenu(int id)
{
	switch(id)
	{
	case 33 :
		exit(0);
	}
}

void createMenu(void)
{
	int m2=glutCreateMenu(bground);
	glutAddMenuEntry("Black",0);
	glutAddMenuEntry("Blue",1);
	glutAddMenuEntry("Green",2);
	glutAddMenuEntry("Cyan",3);
	glutAddMenuEntry("Red",4);
	glutAddMenuEntry("Magenta",5);
	glutAddMenuEntry("Yellow",6);
	glutAddMenuEntry("White",7);
	int m1=glutCreateMenu(mainmenu);
	glutAddSubMenu("Background Color",m2);
	glutAddMenuEntry("Quit",33);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
  glutCreateWindow("CLOCK");
  createMenu();
  glutDisplayFunc(display);
  glutReshapeFunc(myReshape);
  glutTimerFunc(33, TimerFunction, 1);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}