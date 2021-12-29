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
#include "edit.h"
#include "box.h"

#define MAX_ARM 10

using namespace std;

int clickX = 0, clickZ = 0;
int moveX = 0, moveZ = 0;
int prevX = 0, prevZ = 0;
bool leftDown = false;
bool bEdit = false;	// �༭ģʽ����y = 10����
int wWidth, wHeight;	// ��Ļ���

Map map;
Camera camera;
SkyBox sky;
Editor editor;

void renderScene(void)
{
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera.setLook();
	//printf("%f %f %f\n", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	glEnable(GL_LIGHTING);

	GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse_color[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat specular_color[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_pos[] = { BOX_SIZE / 2, 50, BOX_SIZE / 2, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular_color);

	glEnable(GL_LIGHT0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	sky.createSkyBox(BOX_SIZE / 2, 0, BOX_SIZE / 2, 1.0, 0.5, 1.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_COLOR_MATERIAL);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
	//glMaterialf(GL_FRONT, GL_SHININESS, 0.3);
	//glColor3f(1, 0, 0);

	for (Point i = map.getFirst(); i != POINTNULL; i = map.getMap(i).next) {
		Map::MapUnit mu = map.getMap(i);
		if (MAP_ISBELT(mu.type)) {
			((Belt*)(mu.obj))->draw();
			((Belt*)(mu.obj))->updateComponents();
		}
		else if (mu.type == MAP_ARM) {
			((Arm*)(mu.obj))->draw();
		}
		else if (mu.type == MAP_BOX) {
			((Box*)(mu.obj))->draw();
		}
	}

	editor.draw();

	Belt::update();
	if(bEdit) {	// �༭״̬�£�����������ƶ�
		POINT mousePos;
		GetCursorPos(&mousePos);
		if (mousePos.x <= 5)	// ����
			camera.xCamera(-camera.getSpeed());
		if (mousePos.y <= 5)	// ����
			camera.zCamera(-camera.getSpeed());
		if (mousePos.x >= (wWidth - 5))	// ����
			camera.xCamera(camera.getSpeed());
		if (mousePos.y >= (wHeight - 5))	// ����
			camera.zCamera(camera.getSpeed());
		editor.drawMesh(moveZ, moveX);
	}
	glDisable(GL_COLOR_MATERIAL);
	glutSwapBuffers();
}

void updateView(float w, float h) {
	static Vector3 position = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2);		// �������л�ģʽʱ����Ǳ༭ģʽ�������״̬���Ա�ָ�
	static Vector3 view = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1);
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
		camera.setCamera(position.x, 10, position.z, position.x, 1, position.z, 0, 0, -1);
		glOrtho(-5 * ratio, 5 * ratio, -5, 5, 0.1, 400);
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
	float unit = wHeight / 10;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftDown = true;
			clickX = (int)(camera.getPosition().x + (x - wWidth / 2) / unit + 0.5);	// �������λ�ü���������㷨�д��Ż�
			clickZ = (int)(camera.getPosition().z + (y - wHeight / 2) / unit + 0.5);
			printf("Mouse Click: x = %d, z = %d, state = %d\n", clickX, clickZ, editor.getState());
			if (bEdit) {
				if (editor.getState() == EDITOR_STATE_IDLE) {
					editor.startDrawing(clickZ, clickX);
					prevX = clickX, prevZ = clickZ;
				}
				else if (editor.getMode() == EDITOR_MODE_ARM) editor.nextPoint(clickZ, clickX);
				else editor.endDrawing();
			}
			Map::MapUnit mu = map.getMap(clickZ, clickX);
		}
		else leftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (editor.getState() != EDITOR_STATE_IDLE) editor.endDrawing(1);
	}
}

void onMouseMove(int x, int y) {
	float unit = wHeight / 10;
	moveX = (int)(camera.getPosition().x + (x - wWidth / 2) / unit + 0.5);
	moveZ = (int)(camera.getPosition().z + (y - wHeight / 2) / unit + 0.5);

	if (!bEdit) {	// �Ǳ༭ģʽ�£����������仯
		while (ShowCursor(FALSE) >= 0)	// ����������������ӳ��������
			ShowCursor(FALSE);
		camera.setViewByMouse();
	}
	else {
		while (ShowCursor(TRUE) < 0)	// ����������������ӳ���ʾ���
			ShowCursor(TRUE);
	}

	if ((abs(prevZ - moveZ) == 1 && prevX == moveX || abs(prevX - moveX) == 1 && prevZ == moveZ) &&
		editor.getMode() == EDITOR_MODE_BELT && editor.getState() != EDITOR_STATE_IDLE) {
		editor.nextPoint(moveZ, moveX);
		prevX = moveX, prevZ = moveZ;
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
	case '1':
		if (!bEdit) break;
		editor.changeMode(EDITOR_MODE_BELT);
		break;
	case '2':
		if (!bEdit)	break;
		editor.changeMode(EDITOR_MODE_ARM);
		break;
	case '3':
		if (!bEdit)	break;
		editor.changeMode(EDITOR_MODE_DELETE);
		break;
	}

	glutPostRedisplay();
	//printf("key::%d", key);
}

void init() {
	wWidth = glutGet(GLUT_SCREEN_WIDTH);
	wHeight = glutGet(GLUT_SCREEN_HEIGHT);
	//printf("%d %d\n", wWidth, wHeight);

	Belt::init();

	Belt* obj = new Belt();
	obj->pushPoint(0 + 512, 0 + 512);
	obj->pushPoint(1 + 512, 0 + 512);
	obj->pushPoint(1 + 512, 1 + 512);
	obj->pushPoint(2 + 512, 1 + 512);
	obj->pushPoint(2 + 512, 2 + 512);
	obj->pushPoint(1 + 512, 2 + 512);
	obj->pushPoint(1 + 512, 3 + 512);
	obj->pushPoint(1 + 512, 4 + 512);
	obj->pushPoint(0 + 512, 4 + 512);
	obj->pushPoint(0 + 512, 3 + 512);
	obj->pushPoint(0 + 512, 2 + 512);
	obj->updateMap();
	obj->addComponent(new Robot(1));

	Belt* obj2 = new Belt();
	obj2->pushPoint(5 + 512, 5 + 512);
	obj2->pushPoint(6 + 512, 5 + 512);
	obj2->pushPoint(7 + 512, 5 + 512);
	obj2->pushPoint(7 + 512, 6 + 512);
	obj2->pushPoint(7 + 512, 7 + 512);
	obj2->pushPoint(6 + 512, 7 + 512);
	obj2->pushPoint(5 + 512, 7 + 512);
	obj2->pushPoint(5 + 512, 6 + 512);
	obj2->pushPoint(5 + 512, 5 + 512);
	obj2->updateMap();
	obj2->addComponent(new Robot(1));

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	if (!sky.init()){
		MessageBox(NULL, (LPCWSTR)"Fail to initialize skybox!", (LPCWSTR)"Error", MB_OK);
		exit(0);
	}

	//camera.setCamera(BOX_SIZE / 2, 1, BOX_SIZE / 2, BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1, 0, 1, 0);

	Box* box = new Box(512, 0.2, 517);
	box->setType(1);
	box->setRType(1);
	map.write(517, 512, MAP_BOX, box);

	box = new Box(512, 0.2, 521);
	box->setType(1);
	box->setRType(3);
	map.write(521, 512, MAP_BOX, box);

	box = new Box(514, 0.2, 519);
	box->setType(1);
	box->setRType(5);
	map.write(519, 514, MAP_BOX, box);

	box = new Box(510, 0.2, 519);
	box->setType(1);
	box->setRType(7);
	map.write(519, 510, MAP_BOX, box);

	box = new Box(512, 0.2, 519);
	box->setType(2);
	map.write(519, 512, MAP_BOX, box);
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
