#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include "belt.h"
#include "skybox.h"
#include "camera.h"

int OriX = -1, OriY = -1;
bool rightDown = 0;
bool bEdit = false;	// 编辑模式，从y = 100俯视

Belt obj;
Camera camera;
SkyBox sky;

void renderScene(void)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_LIGHTING);

	GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT0);

	camera.setLook();

	sky.CreateSkyBox(0, 0, 0, 1.0, 0.5, 1.0);

	obj.draw();
	Belt::update();

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

void onMouseMove(int x, int y){
	if(!bEdit)	// 非编辑模式下，视线随鼠标变化
		camera.setViewByMouse();

	glutPostRedisplay();
}

void key(unsigned char k, int x, int y){
	switch (k) {
	case 27:
		exit(0);
		break;
	case '1':
		camera.setSpeed(0.2f);
		break;
	case '2':
		camera.setSpeed(1.0f);
		break;
	case 'w':
		camera.moveCamera(camera.getSpeed());
		break;
	case 's':
		camera.moveCamera(-camera.getSpeed());
		break;
	case 'a':
		camera.yawCamera(-camera.getSpeed());
		break;
	case 'd':
		camera.yawCamera(camera.getSpeed());
		break;
	case 'e':
		bEdit = !bEdit;
		break;
	}

	glutPostRedisplay();
	//printf("key::%d", key);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.1, 4000.0);
	// glOrtho(-3, 3, -3, 3, -1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void init() {
	Belt::init();
	obj.pushPoint(-1, 0);
	obj.pushPoint(0, 0);
	obj.pushPoint(1, 0);
	obj.pushPoint(1, 1);
	obj.pushPoint(2, 1);
	obj.pushPoint(2, 2);
	obj.pushPoint(1, 2);
	obj.pushPoint(1, 3);
	obj.pushPoint(1, 4);
	obj.pushPoint(0, 4);
	obj.pushPoint(0, 3);
	obj.pushPoint(0, 2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	if (!sky.Init()){
		MessageBox(NULL, (LPCWSTR)"Fail to initialize skybox!", (LPCWSTR)"Error", MB_OK);
		exit(0);
	}

	camera.setCamera(0, 1, 1, 0, 1, 0, 0, 1, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("CG Project");
	glutFullScreen();

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(onMouseMove);
	glutKeyboardFunc(key);

	glutMainLoop();
	return 0;
}
