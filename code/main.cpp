#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include "map.h"
#include "belt.h"
#include "skybox.h"
#include "camera.h"
#include "arm.h"
#include "component.h"

#define MAX_ARM 10

using namespace std;

int OriX = -1, OriY = -1;
bool rightDown = 0;
bool bEdit = false;	// �༭ģʽ����y = 10����
float wWidth, wHeight;

Map map;
Belt belt;
Camera camera;
SkyBox sky;
Arm arm[MAX_ARM];
Robot robot;

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
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT0);

	camera.setLook();

	sky.CreateSkyBox(0, 0, 0, 1.0, 0.5, 1.0);

	for (Point i = map.getFirst(); i != POINTNULL; i = map.getMap(i).next) {
		Map::MapUnit mu = map.getMap(i);
		if (mu.type >= MAP_BELT_CORNER0 && mu.type <= MAP_BELT_SINGLE) {
			((Belt*)(mu.obj))->draw();
		}
	}

	if(!bEdit)	Belt::update();	// �Ǳ༭״̬�£����ʹ��ƶ�

	glPushMatrix();
	glTranslatef(0, 0, 1);
	robot.draw();
	glPopMatrix();

	arm[0].Draw();

	glutSwapBuffers();
}

void updateView(float w, float h) {
	static Vector3 position = Vector3(0, 1, 1);		// �������л�ģʽʱ����Ǳ༭ģʽ�������״̬���Ա�ָ�
	static Vector3 view = Vector3(0, 1, 0);
	static Vector3 upVector = Vector3(0, 1, 0);
	float ratio = w / h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!bEdit) {	// �Ǳ༭ģʽ
		camera.setCamera(position.x, position.y, position.z, view.x, view.y, view.z, upVector.x, upVector.y, upVector.z);
		gluPerspective(45.0, ratio, 0.1, 4000.0);
	}
	else {	// �༭ģʽ
		position = camera.getPosition();
		view = camera.getView();
		upVector = camera.getUpVector();
		camera.setCamera(0, 10, 0, 0, 1, 0, 0, 0, -1);
		glOrtho(-5 * ratio, 5 * ratio, -5, 5, -1, 10);
	}
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void reshape(int w, int h)
{
	if (h == 0)	h = 1;
	updateView(wWidth, wHeight);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			rightDown = 1;
			OriX = x, OriY = y;
		}
		else rightDown = 0;
	}
}

void onMouseMove(int x, int y) {
	if (!bEdit) {	// �Ǳ༭ģʽ�£����������仯
		while (ShowCursor(FALSE) >= 0)	// ����������������ӳ��������
			ShowCursor(FALSE);
		camera.setViewByMouse();
	}
	else {
		while (ShowCursor(TRUE) < 0)	// ����������������ӳ���ʾ���
			ShowCursor(TRUE);
	}

	glutPostRedisplay();
}

void key(unsigned char k, int x, int y) {
	switch (k) {
	case 27:
		exit(0);
		break;
	case 'w':
		if (bEdit)	break;
		camera.moveCamera(camera.getSpeed());
		break;
	case 's':
		if (bEdit)	break;
		camera.moveCamera(-camera.getSpeed());
		break;
	case 'a':
		if (bEdit)	break;
		camera.yawCamera(-camera.getSpeed());
		break;
	case 'd':
		if (bEdit)	break;
		camera.yawCamera(camera.getSpeed());
		break;
	case ' ':
		if (bEdit)	break;
		camera.liftCamera(camera.getSpeed());
		break;
	case 'e':
		bEdit = !bEdit;
		updateView(wWidth, wHeight);	// ������˳��༭ģʽ����Ҫ���������λ��
		break;
	}

	glutPostRedisplay();
	//printf("key::%d", key);
}

void init() {
	wWidth = glutGet(GLUT_SCREEN_WIDTH);
	wHeight = glutGet(GLUT_SCREEN_HEIGHT);

	Belt::init();

	Belt* obj = new Belt();
	obj->pushPoint(0, 0);
	obj->pushPoint(1, 0);
	obj->pushPoint(1, 1);
	obj->pushPoint(2, 1);
	obj->pushPoint(2, 2);
	obj->pushPoint(1, 2);
	obj->pushPoint(1, 3);
	obj->pushPoint(1, 4);
	obj->pushPoint(0, 4);
	obj->pushPoint(0, 3);
	obj->pushPoint(0, 2);
	obj->pushPoint(0, 1);
	obj->pushPoint(0, 0);
	obj->updateMap();

	obj->delPoint(0);
	Belt* obj1 = obj->delPoint(3);
	obj->delPoint(2);
	//obj->print();
	//obj1->print();
	obj->merge(obj1);
	obj->print();
	obj->delPoint(1);

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

	Arm::init();
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
