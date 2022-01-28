#include <windows.h>
#include <time.h>
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
#include "obj.h"

#define MAX_ARM 10

#define GETMESH_X(a) ((int)(camera.getPosition().x + ((a) - (wWidth + menuWidth) / 2) / meshUnit + 0.5))
#define GETMESH_Z(b) ((int)(camera.getPosition().z + ((b) - wHeight / 2) / meshUnit + 0.5))

using namespace std;

int clickX = 0, clickZ = 0;
int moveX = 0, moveZ = 0;
int prevX = 0, prevZ = 0;
bool leftDown = false;
bool bEdit = false;	// �༭ģʽ����y = 10����
bool bW, bA, bS, bD, bPlus, bMinus, bSpace;
int wWidth, wHeight, menuWidth;	// ��Ļ��ߣ��˵���
const float menuWidthRatio = 0.2;
float meshUnit;	// �༭ģʽ������߳�

Map map;
Camera camera;
SkyBox sky;
Editor editor;
Menu menu;
Vector3 position = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2);		// �������л�ģʽʱ����Ǳ༭ģʽ�������״̬���Ա�ָ�
Vector3 view = Vector3(BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1);
Vector3 upVector = Vector3(0, 1, 0);
ObjLoader *pic[9];

inline int random(int l, int r) { return rand() % (r - l + 1) + l; }

void updateView(float w, float h, bool save = 1) {
	
	float ratio = w / h;
	if (!bEdit) {	// �Ǳ༭ģʽ
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(camera.getFovy(), ratio, 0.1, 4000.0);
	}
	else {	// �༭ģʽ
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

	if (!bEdit) {
		Vector3 curP = camera.getPosition();
		bool check = map.checkEdge(curP.x, curP.y, curP.z);
		int type = map.getMap((int)(curP.z + 0.5), (int)(curP.x + 0.5)).type;
		camera.updateHeight(check, map.getFloor(curP.x, curP.z));
		processKeyboard();
	}
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

	map.drawGround();

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
		if (!bEdit) {
			float newY = map.getFloor(position.x, position.z);
			view.y += newY - position.y;
			position.y = newY;
			camera.setCamera(position.x, position.y, position.z, view.x, view.y, view.z, upVector.x, upVector.y, upVector.z);
			SetCursorPos(GetSystemMetrics(SM_CXSCREEN) >> 1, GetSystemMetrics(SM_CYSCREEN) >> 1);
		}
		updateView(wWidth, wHeight);	// ������˳��༭ģʽ����Ҫ���������λ��
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

	map.init();

	Belt::init();

	menu.init();

	new LightSource(BOX_SIZE / 2, BOX_SIZE / 2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	if (!sky.init()){
		printf("Fail to initialize skybox!\n");
		exit(0);
	}

	camera.setCamera(BOX_SIZE / 2, 0.8, BOX_SIZE / 2, BOX_SIZE / 2, 1, BOX_SIZE / 2 - 1, 0, 1, 0);

	srand(time(NULL));
	static const int componentRange[4][2] = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
	int startNum[4];
	int startComponent[4][2];
	int startPos[4][2][2], endPos[2][2];
	Box* box;

	// starting boxes
	for (int i = 0; i < 4; i++) { // for each category
		startNum[i] = random(1, 2); // randomly generate 1 or 2 boxes
		for (int j = 0; j < startNum[i]; j++) {
			startComponent[i][j] = random(componentRange[i][0], componentRange[i][1]); // randomly determine component type
			while (1) {	// randomly select position in 20 * 20 range
				startPos[i][j][0] = random(502, 522);
				startPos[i][j][1] = random(502, 522);
				if (map.getMap(startPos[i][j][0], startPos[i][j][1]).type == MAP_BLANK) break;
			}
			// create box
			box = new Box(startPos[i][j][1], 0.2, startPos[i][j][0]);
			box->setType(1);
			box->setRType(startComponent[i][j]);
			map.write(startPos[i][j][0], startPos[i][j][1], MAP_BOX, box);
		}
	}

	// first ending box
	while (1) {	// randomly select position in 20 * 20 range
		endPos[0][0] = random(502, 522);
		endPos[0][1] = random(502, 522);
		if (map.getMap(endPos[0][0], endPos[0][1]).type == MAP_BLANK) break;
	}
	box = new Box(endPos[0][1], 0.2, endPos[0][0]);
	box->setNeed(0, startComponent[0][0]);						// head, use starting box 0
	box->setNeed(1, startComponent[1][0]);						// body, use starting box 0
	box->setNeed(2, startComponent[2][0]);						// arm1, use starting box 0
	if (startNum[2] > 1) box->setNeed(3, startComponent[2][1]);	// arm2, if start num > 1 use box 1,else use starting box 0
	else box->setNeed(3, startComponent[2][0]);
	box->setNeed(4, startComponent[3][0]);						// leg1, use starting box 0
	if (startNum[3] > 1) box->setNeed(5, startComponent[3][1]);	// leg2, if start num > 1 use box 1,else use starting box 0
	else box->setNeed(5, startComponent[3][0]);
	box->setType(2);
	map.write(endPos[0][0], endPos[0][1], MAP_BOX, box);

	// if head or body starting box type > 1, generate second ending box
	if (startNum[0] > 1 && startComponent[0][0] != startComponent[0][1] ||
		startNum[1] > 1 && startComponent[1][0] != startComponent[1][1]) {
		while (1) {	// randomly select position in 20 * 20 range
			endPos[1][0] = random(502, 522);
			endPos[1][1] = random(502, 522);
			if (map.getMap(endPos[1][0], endPos[1][1]).type == MAP_BLANK) break;
		}
		box = new Box(endPos[1][1], 0.2, endPos[1][0]);
		if (startNum[0] > 1) box->setNeed(0, startComponent[0][1]);		// head, if start num > 1 use box 1,else use starting box 0
		else box->setNeed(0, startComponent[0][0]);
		if (startNum[1] > 1) box->setNeed(1, startComponent[1][1]);		// body, if start num > 1 use box 1,else use starting box 0
		else box->setNeed(1, startComponent[1][0]);
		box->setNeed(2, startComponent[2][random(0, startNum[2] - 1)]);	// arm1, use random component
		box->setNeed(3, startComponent[2][random(0, startNum[2] - 1)]);	// arm2, use random component
		box->setNeed(4, startComponent[3][random(0, startNum[3] - 1)]);	// leg1, use random component
		box->setNeed(5, startComponent[3][random(0, startNum[3] - 1)]);	// leg2, use random component
		box->setType(2);
		map.write(endPos[1][0], endPos[1][1], MAP_BOX, box);
	}

	pic[1] = new ObjLoader("obj/head1.obj");
	pic[2] = new ObjLoader("obj/head2.obj");
	pic[3] = new ObjLoader("obj/body1.obj");
	pic[4] = new ObjLoader("obj/body2.obj");
	pic[5] = new ObjLoader("obj/arm1.obj");
	pic[6] = new ObjLoader("obj/arm2.obj");
	pic[7] = new ObjLoader("obj/leg2.obj");
	pic[8] = new ObjLoader("obj/leg2.obj");
	pic[1]->Setscale(0.025);
	pic[2]->Setscale(0.025);
	pic[3]->Setscale(0.3);
	pic[4]->Setscale(0.038);
	pic[5]->Setscale(0.04);
	pic[6]->Setscale(0.08);
	pic[7]->Setscale(0.045);
	pic[8]->Setscale(0.06);
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
	glutIgnoreKeyRepeat(1);	// ���ü����ظ���ʵ��ͬʱ��������������
	glutReshapeFunc(reshape);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(onMouseMove);
	glutKeyboardFunc(keyDown);	// ���̰���ʱ�ص�
	glutKeyboardUpFunc(keyUp);	// �����ɿ�ʱ�ص�

	glutMainLoop();
	return 0;
}
