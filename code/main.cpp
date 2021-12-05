#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "belt.h"
#include <gl/glut.h>
int OriX = -1, OriY = -1;
bool rightDown = 0;
const float C = acos(-1) / 180;
const float r = 3;
const float moveStep = 0.2;
float eye[] = {0, 1, 2}, du[] = {120, 180};

Belt obj0, obj1;

void renderScene(void)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],
			  eye[0] + r * sin(C * du[0]) * sin(C * du[1]), eye[1] + r * cos(C * du[0]), eye[2] + r * sin(C * du[0]) * cos(C * du[1]),
			  0.0, 1.0, 0.0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT0);

	obj1.draw(1);

	glPushMatrix();
	glTranslatef(-1, 0, 1);
	glRotatef(90, 0, 1, 0);
	obj0.draw(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0, 1);
	glRotatef(90, 0, 1, 0);
	obj0.draw(0);
	glPopMatrix();

	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON){
		if(state == GLUT_DOWN){
			rightDown = 1;
			OriX = x, OriY = y;
		}
		else rightDown = 0;
	}
}

void onMouseMove(int x, int y)
{
	if(rightDown){
		du[1] -= (x - OriX) * 0.2;
		du[0] += (y - OriY) * 0.2;
		if(du[0] < 1) du[0] = 1;
		if(du[0] > 179) du[0] = 179;
		OriX = x, OriY = y;
	}
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case 'a': {
		eye[0] -= moveStep;
		break;
		}
	case 'd': {
		eye[0] += moveStep;
		break;
		}
	case 'w': {
		eye[1] += moveStep;
		break;
		}
	case 's': {
		eye[1] -= moveStep;
		break;
		}
	case 'z': {
		eye[2] -= moveStep;
		break;
		}
	case 'c': {
		eye[2] += moveStep;
		break;
		}
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.001, 1000.0);
	// glOrtho(-3, 3, -3, 3, -1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void init() {
	Belt::init();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("CG Project");

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	glutKeyboardFunc(key);

	glutMainLoop();
	return 0;
}
