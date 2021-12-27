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
#include "edit.h"

#define MAX_ARM 10

using namespace std;

int OriX = -1, OriY = -1;
bool leftDown = false;
bool bEdit = false;	// 编辑模式，从y = 10俯视
int wWidth, wHeight;

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
	GLfloat light_pos[] = { BOX_SIZE / 2, 10, BOX_SIZE / 2, 1 };

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

	glPushMatrix();
	glPopMatrix();

	for (Point i = map.getFirst(); i != POINTNULL; i = map.getMap(i).next) {
		Map::MapUnit mu = map.getMap(i);
		if (MAP_ISBELT(mu.type)) {
			((Belt*)(mu.obj))->draw();
			((Belt*)(mu.obj))->updateComponents();
		}
		else if (mu.type == MAP_ARM) {
			((Arm*)(mu.obj))->Draw();
		}
	}

	Belt::update();
	if(bEdit) {	// 编辑状态下，摄像机可能移动
		POINT mousePos;
		GetCursorPos(&mousePos);
		if (mousePos.x <= 5)	// 左移
			camera.xCamera(-camera.getSpeed());
		if (mousePos.y <= 5)	// 上移
			camera.zCamera(-camera.getSpeed());
		if (mousePos.x >= (wWidth - 5))	// 右移
			camera.xCamera(camera.getSpeed());
		if (mousePos.y >= (wHeight - 5))	// 下移
			camera.zCamera(camera.getSpeed());
		editor.drawMesh();
	}
	glDisable(GL_COLOR_MATERIAL);
	glutSwapBuffers();
}

void updateView(float w, float h) {
	static Vector3 position = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2);		// 用于在切换模式时保存非编辑模式下摄像机状态，以便恢复
	static Vector3 view = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1);
	static Vector3 upVector = Vector3(0, 1, 0);
	float ratio = w / h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!bEdit) {	// 非编辑模式
		camera.setCamera(position.x, position.y, position.z, view.x, view.y, view.z, upVector.x, upVector.y, upVector.z);
		gluPerspective(45.0, ratio, 0.1, 4000.0);
	}
	else {	// 编辑模式
		position = camera.getPosition();
		view = camera.getView();
		upVector = camera.getUpVector();
		camera.setCamera(position.x, 10, position.z, position.x, 1, position.z, 0, 0, -1);
		printf("%f %f %f\n", camera.getView().x, camera.getView().y, camera.getView().z);
		//gluPerspective(45.0, ratio, 0.1, 400);	// 透视投影yyds
		glOrtho(position.x - 5 * ratio, position.x + 5 * ratio, position.z - 5, position.z + 5, 0.1, 400);	// 正交投影为什么会对不准中心啊
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
	int unit = wHeight / 10;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftDown = true;
			OriX = x, OriY = y;
			printf("%d %d\n", (int)(camera.getPosition().z + (y - wHeight / 2) / unit), (int)(camera.getPosition().x + (x - wWidth / 2)) / unit);
			//editor.beltAddPoint((int)(camera.getPosition().z + (y - wHeight / 2) / unit), (int)(camera.getPosition().x + (x - wWidth / 2)) / unit);
		}
		else leftDown = false;
	}
}

void onMouseMove(int x, int y) {
	if (!bEdit) {	// 非编辑模式下，视线随鼠标变化
		while (ShowCursor(FALSE) >= 0)	// 清除计数器，避免延迟隐藏鼠标
			ShowCursor(FALSE);
		camera.setViewByMouse();
	}
	else {
		while (ShowCursor(TRUE) < 0)	// 清除计数器，避免延迟显示鼠标
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
		updateView(wWidth, wHeight);	// 进入或退出编辑模式，需要更新摄像机位置
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
	//obj->print();
	obj->addComponent(new Robot(1));

	Belt* obj2 = new Belt();
	obj2->pushPoint(5, 5);
	obj2->pushPoint(6, 5);
	obj2->pushPoint(7, 5);
	obj2->pushPoint(7, 6);
	obj2->pushPoint(7, 7);
	obj2->pushPoint(6, 7);
	obj2->pushPoint(5, 7);
	obj2->pushPoint(5, 6);
	obj2->pushPoint(5, 5);
	obj2->updateMap();
	obj2->addComponent(new Robot(1));
	obj2->setColor(BELT_COLOR_WARNING);

	Belt* obj3 = new Belt();
	obj3->pushPoint(0, 6);
	obj3->pushPoint(0, 7);
	obj3->pushPoint(0, 8);
	obj3->pushPoint(0, 9);
	obj3->pushPoint(1, 9);
	obj3->pushPoint(1, 8);
	obj3->pushPoint(2, 8);
	obj3->pushPoint(3, 8);
	obj3->pushPoint(3, 9);
	obj3->pushPoint(4, 9);
	obj3->pushPoint(4, 8);
	obj3->pushPoint(4, 7);
	obj3->pushPoint(4, 6);
	obj3->pushPoint(3, 6);
	obj3->pushPoint(3, 7);
	obj3->pushPoint(2, 7);
	obj3->pushPoint(2, 6);
	obj3->pushPoint(1, 6);
	obj3->pushPoint(0, 6);
	obj3->updateMap();
	obj3->addComponent(new Robot(1));
	obj3->setColor(BELT_COLOR_DRAWING);

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

	Arm* arm = new Arm(0, 5, 3, 1);
	map.write(0, 5, MAP_ARM, arm);
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
