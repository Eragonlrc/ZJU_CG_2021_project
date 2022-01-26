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
#include "menu.h"
#include "lighting.h"

#define MAX_ARM 10

#define GETMESH_X(a) ((int)(camera.getPosition().x + ((a) - (wWidth + menuWidth) / 2) / meshUnit + 0.5))
#define GETMESH_Z(b) ((int)(camera.getPosition().z + ((b) - wHeight / 2) / meshUnit + 0.5))

using namespace std;

int clickX = 0, clickZ = 0;
int moveX = 0, moveZ = 0;
int prevX = 0, prevZ = 0;
bool leftDown = false;
bool bEdit = false;	// 编辑模式，从y = 10俯视
bool bW, bA, bS, bD, bPlus, bMinus, bSpace;
int wWidth, wHeight, menuWidth;	// 屏幕宽高，菜单宽
const float menuWidthRatio = 0.2;
float meshUnit;	// 编辑模式下网格边长

Map map;
Camera camera;
SkyBox sky;
Editor editor;
Menu menu;
Vector3 position = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2);		// 用于在切换模式时保存非编辑模式下摄像机状态，以便恢复
Vector3 view = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1);
Vector3 upVector = Vector3(0, 1, 0);

void updateView(float w, float h, bool save = 1) {
	
	float ratio = w / h;
	if (!bEdit) {	// 非编辑模式
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(camera.getFovy(), ratio, 0.1, 4000.0);
	}
	else {	// 编辑模式
		glViewport(w * menuWidthRatio, 0, w * (1 - menuWidthRatio), h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (save) {
			position = camera.getPosition();
			view = camera.getView();
			upVector = camera.getUpVector();
			camera.setCamera(position.x, 10, position.z, position.x, 1, position.z, 0, 0, -1);
		}
		glOrtho(-5 * ratio * (1 - menuWidthRatio), 5 * ratio * (1 - menuWidthRatio), -5, 5, 0.1, 400);
	}
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void processKeyboard() {
	Vector3 nextP;	// the next position of camera
	if (bW) {
		camera.moveCamera(camera.getSpeed());
		nextP = camera.getPosition();
		if (!map.checkEdge(nextP.x, nextP.y, nextP.z))	// sth. at the next position
			camera.moveCamera(-camera.getSpeed());
	}
	if (bS) {
		camera.moveCamera(-camera.getSpeed());
		nextP = camera.getPosition();
		if (!map.checkEdge(nextP.x, nextP.y, nextP.z))	// sth. at the next position
			camera.moveCamera(camera.getSpeed());
	}
	if (bA) {
		camera.yawCamera(-camera.getSpeed());
		nextP = camera.getPosition();
		if (!map.checkEdge(nextP.x, nextP.y, nextP.z))	// sth. at the next position
			camera.yawCamera(camera.getSpeed());
	}
	if (bD) {
		camera.yawCamera(camera.getSpeed());
		nextP = camera.getPosition();
		if (!map.checkEdge(nextP.x, nextP.y, nextP.z))	// sth. at the next position
			camera.yawCamera(-camera.getSpeed());
	}
	if (bPlus) {
		camera.updateFovy(-1.0);
		updateView(wWidth, wHeight, 1);
	}
	if (bMinus) {
		camera.updateFovy(1.0);
		updateView(wWidth, wHeight, 1);
	}
	if (bSpace) {
		if (!camera.getJumping())
			camera.liftCamera(camera.getSpeed());
	}
}

void renderScene(void)
{
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (bEdit) { // draw menu
		menu.setView(wWidth, wHeight);
		menu.draw();
		updateView(wWidth, wHeight, 0);
	}
	glLoadIdentity();

	if(!bEdit)
		processKeyboard();

	Vector3 curP = camera.getPosition();
	bool check = map.checkEdge(curP.x, curP.y, curP.z);
	int type = map.getMap((int)(curP.z + 0.5), (int)(curP.x + 0.5)).type;
	camera.updateHeight(check, MAP_ISBELT(type));
	camera.setLook();

	glEnable(GL_LIGHTING);
	LightSource::enableAll();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	sky.createSkyBox(BOX_SIZE / 2, 0, BOX_SIZE / 2, 1.0, 0.5, 1.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_COLOR_MATERIAL);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
	//glMaterialf(GL_FRONT, GL_SHININESS, 0.3);
	//glColor3f(1, 0, 0);

	LightSource::drawAll();

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
		editor.drawMesh(moveZ, moveX);
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	if (h == 0)	h = 1;
	wWidth = w, wHeight = h;
	menuWidth = w * menuWidthRatio;
	menu.setWH((float)wWidth / wHeight * menuWidthRatio);
	updateView(wWidth, wHeight);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftDown = true;
			if (bEdit && x <= menuWidth) { // click menu
				menu.click((float)x / wHeight * 2 - (float)menuWidth / wHeight, 1 - (float)y / wHeight * 2);
				return;
			}
			clickX = GETMESH_X(x);	// account for the width of menu
			clickZ = GETMESH_Z(y);
			printf("Mouse Click: x = %d, z = %d\n", clickX, clickZ);
			if (bEdit) {
				if (editor.getState() == EDITOR_STATE_IDLE) {
					editor.startDrawing(clickZ, clickX);
					prevX = clickX, prevZ = clickZ;
				}
				else if (editor.getMode() == EDITOR_MODE_ARM) editor.nextPoint(clickZ, clickX);
				else editor.endDrawing();
			}
			/*Map::MapUnit mu = map.getMap(clickZ, clickX);
			printf("first: (%d, %d), last: (%d, %d)\n", map.getFirst().first, map.getFirst().second, map.getLast().first, map.getLast().second);
			for (Point i = map.getFirst(); i != POINTNULL; i = map.getMap(i).next) {
				Map::MapUnit mu = map.getMap(i);
				printf("(%d, %d) type = %d\n", i.first, i.second, mu.type);
			}*/
		}
		else leftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (editor.getState() != EDITOR_STATE_IDLE) editor.endDrawing(1);
	}
}

void onMouseMove(int x, int y) {
	moveX = GETMESH_X(x);	// account for the width of menu
	moveZ = GETMESH_Z(y);

	if (!bEdit) {	// 非编辑模式下，视线随鼠标变化
		while (ShowCursor(FALSE) >= 0)	// 清除计数器，避免延迟隐藏鼠标
			ShowCursor(FALSE);
		camera.setViewByMouse();
	}
	else {
		while (ShowCursor(TRUE) < 0)	// 清除计数器，避免延迟显示鼠标
			ShowCursor(TRUE);
	}

	if ((abs(prevZ - moveZ) == 1 && prevX == moveX || abs(prevX - moveX) == 1 && prevZ == moveZ) &&
		editor.getMode() == EDITOR_MODE_BELT && editor.getState() != EDITOR_STATE_IDLE) {
		editor.nextPoint(moveZ, moveX);
		prevX = moveX, prevZ = moveZ;
	}

	glutPostRedisplay();
}

void keyDown(unsigned char k, int x, int y) {
	Vector3 nextP;	// the next position of camera
	switch (k) {
	case 27:
		exit(0);
		break;
	case 'w':	bW = true; break;
	case 's':	bS = true; break;
	case 'a':	bA = true; break;
	case 'd':	bD = true; break;
	case '-':	bMinus = true; break;
	case '+':	bPlus = true; break;
	case ' ':	bSpace = true; break;
	case 'e':
		bEdit = !bEdit;
		if (!bEdit)	camera.setCamera(position.x, position.y, position.z, view.x, view.y, view.z, upVector.x, upVector.y, upVector.z);
		updateView(wWidth, wHeight);	// 进入或退出编辑模式，需要更新摄像机位置
		break;
	}

	glutPostRedisplay();
}

void keyUp(unsigned char k, int x, int y) {
	switch (k) {
	case 'w':	bW = false; break;
	case 's':	bS = false; break;
	case 'a':	bA = false; break;
	case 'd':	bD = false; break;
	case '-':	bMinus = false; break;
	case '+':	bPlus = false; break;
	case ' ':	bSpace = false; break;
	}
}

void init() {
	wWidth = glutGet(GLUT_SCREEN_WIDTH);
	wHeight = glutGet(GLUT_SCREEN_HEIGHT);
	menuWidth = wWidth * menuWidthRatio;
	menu.setWH((float)wWidth / wHeight * menuWidthRatio);
	meshUnit = wHeight / 10;

	Belt::init();

	menu.init();

	GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse_color[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat specular_color[] = { 0.5, 0.5, 0.5, 1.0 };
	new LightSource(BOX_SIZE / 2, BOX_SIZE / 2);

	Belt* obj = new Belt();
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

	camera.setCamera(BOX_SIZE / 2, 0.8, BOX_SIZE / 2, BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1, 0, 1, 0);

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
	glutIgnoreKeyRepeat(1);	// 禁用键盘重复，实现同时处理多个键盘输入
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(onMouseMove);
	glutKeyboardFunc(keyDown);	// 键盘按下时回调
	glutKeyboardUpFunc(keyUp);	// 键盘松开时回调

	glutMainLoop();
	return 0;
}
