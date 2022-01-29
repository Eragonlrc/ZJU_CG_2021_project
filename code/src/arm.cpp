#include "arm.h"
#include <math.h>
extern Map map;
extern Belt belt;

Arm::Arm(int dx, int dy, int f, int t) {
	x = dx;
	y = dy;
	state = 0;
	from = f;
	to = t;
	phase = 0;
	arm1[0] = f * 90.0;
	arm1[1] = 3.0;
	arm2[0] = 0.0;
	arm2[1] = -3.0;
	arm3[0] = 0.0;
	arm3[1] = -28.0;
	clawAngle = 55.0;
	showAttachment = false;
	clockWise = 1;
	robot = NULL;
	switch (t)
	{
	case 0: {tx = x + 1; ty = y; break; }
	case 1: {tx = x; ty = y + 1; break; }
	case 2: {tx = x - 1; ty = y; break; }
	case 3: {tx = x; ty = y - 1; break; }
	default: break;
	}
	if (t - f == 1 || (t == 0 && f == 4)) clockWise = -1;
	armListId = glGenLists(11);
	glNewList(ARM_FOUNDATION_SPHERE, GL_COMPILE);
	mySphere(0.3);
	glEndList();
	glNewList(ARM_JOINT1_SPHERE, GL_COMPILE);
	mySphere(0.27);
	glEndList();
	glNewList(ARM_JOINT2_SPHERE, GL_COMPILE);
	mySphere(0.25);
	glEndList();
	glNewList(ARM_CLAW_ROOT_SPHERE, GL_COMPILE);
	mySphere(0.12);
	glEndList();
	glNewList(ARM_CLAW_JOINT1_SPHERE, GL_COMPILE);
	mySphere(0.02);
	glEndList();
	glNewList(ARM_CLAW_END_SPHERE, GL_COMPILE);
	mySphere(0.015);
	glEndList();
	glNewList(ARM_1_CYLINDER, GL_COMPILE);
	myCylinder(0.23, 0.18, 1.2);
	glEndList();
	glNewList(ARM_2_CYLINDER, GL_COMPILE);
	myCylinder(0.18, 0.15, 1.0);
	glEndList();
	glNewList(ARM_3_CYLINDER, GL_COMPILE);
	myCylinder(0.18, 0.12, 0.7);
	glEndList();
	glNewList(ARM_CLAW_1_CYLINDER, GL_COMPILE);
	myCylinder(0.04, 0.02, 0.3);
	glEndList();
	glNewList(ARM_CLAW_2_CYLINDER, GL_COMPILE);
	myCylinder(0.02, 0.015, 0.2);
	glEndList();
}

void Arm::update() {
	/*
		state	operation
		0		ready
		1		bend
		2		fetch
		3		lift
		4		rotate
		5		release
		6 and 7	reset
	*/
	if (state == 1) {
		if (abs(arm3[1] - (-28.0)) > 0.05) arm3[1] -= 0.2;
		else if (abs(arm1[1] - 3.0) > 0.05)	arm1[1] += 0.2;
		else if (abs(arm2[1] - (-3.0)) > 0.05) arm2[1] += 0.2;
		else if (phase == 0) {
			state = 2;
			phase = 1;
		}
		else {
			state = 5;
			phase = 0;
		}
	}
	if (state == 2) {
		if (clawAngle > 30.0) clawAngle -= 1.0;
		else {
			state = 3;
			showAttachment = true;
		}
	}
	if (state == 3) {
		if (abs(arm1[1] - (-20.0)) > 0.05)	arm1[1] -= 0.2;
		else if (abs(arm2[1] - (-35.0)) > 0.05) arm2[1] -= 0.2;
		else if (abs(arm3[1] - (-28.0)) > 0.05) arm3[1] += 0.2;
		else state = 4;
	}
	if (state == 4) {
		if (abs(arm1[0] - to * 90) > 0.5 || (to == 0 && (arm1[0] < 359.0 && arm1[0] > 1.0)))	arm1[0] -= clockWise * 0.5;
		else state = 1;
	}
	if (state == 5) {
		Map::MapUnit mu = map.getMap(tx, ty);
		if (mu.type == MAP_BOX && ((Box*)(mu.obj))->getReady() == false)	return;
		if (clawAngle < 60.0) clawAngle += 1.0;
		else {
			updateItem();
			state = 6;
			showAttachment = false;
		}
	}
	if (state == 6) {
		if (abs(arm1[1] - (-20.0)) > 0.05) arm1[1] -= 0.2;
		else if (abs(arm1[0] - from * 90) > 1.0 || (from == 0 && (arm1[0] < 359.0 && arm1[0] > 1.0)))	arm1[0] += clockWise * 0.5;
		else state = 7;
	}
	if (state == 7) {
		if (abs(arm1[1] - 3.0) > 0.05)	arm1[1] += 0.2;
		else if (abs(arm2[1] - (-3.0)) > 0.05) arm2[1] += 0.2;
		else if (abs(arm3[1] - (-28.0)) > 0.05) arm3[1] += 0.2;
		else state = 0;
	}
	if (arm1[0] > 360.0)	arm1[0] -= 360.0;
	if (arm1[0] < 0)	arm1[0] += 360.0;
	if (arm2[0] > 360.0)	arm2[0] -= 360.0;
	if (arm3[0] > 360.0)	arm3[0] -= 360.0;
}

void Arm::drawFoundation() {
	glScalef(1.0, 0.4, 1.0);
	glutSolidCube(0.7);
	glScalef(1.0, 2.5, 1.0);
}

void Arm::drawJoint1() {
	glTranslatef(0.0, 0.15, 0.0);
	glCallList(ARM_FOUNDATION_SPHERE);
	glTranslatef(0.0, -0.15, 0.0);
}

void Arm::drawArm1() {
	glCallList(ARM_1_CYLINDER);
}

void Arm::drawArm2() {
	glCallList(ARM_JOINT1_SPHERE);
	glCallList(ARM_2_CYLINDER);
}

void Arm::drawArm3() {
	glCallList(ARM_JOINT2_SPHERE);
	glCallList(ARM_3_CYLINDER);
}

void Arm::drawClaw() {
	glCallList(ARM_CLAW_ROOT_SPHERE);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glRotatef(120 * i, 0, 0, 1);
		glRotatef(clawAngle, 0, -1, 0);
		glCallList(ARM_CLAW_1_CYLINDER);
		glTranslatef(0.0, 0.0, 0.3);
		glCallList(ARM_CLAW_JOINT1_SPHERE);
		glRotatef(45, 0, 1, 0);
		glCallList(ARM_CLAW_2_CYLINDER);
		glTranslatef(0.0, 0.0, 0.2);
		glCallList(ARM_CLAW_END_SPHERE);
		glPopMatrix();
	}
}

void Arm::draw() {
	update();
	glPushMatrix();
	switch (color) {
	case ARM_COLOR_DEFAULT: glColor3f(1.0, 1.0, 1.0); break;
	case ARM_COLOR_DRAWING: glColor3f(0.0, 1.0, 0.0); break;
	case ARM_COLOR_WARNING: glColor3f(1.0, 0.0, 0.0); break;
	}
	glTranslatef(y, 0, x);
	glTranslatef(0.0, 0.2, 0.0);
	glScalef(0.6, 0.6, 0.6);
	drawFoundation();
	glPushMatrix();
	glRotatef(arm1[0], 0, 1, 0);
	drawJoint1();
	glPushMatrix();
	glTranslatef(0.0, 0.1, 0.0);
	glRotatef(arm1[1] - 90, 1, 0, 0);
	drawArm1();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.2);
	glRotatef(arm2[0], 0, 0, 1);
	glRotatef(arm2[1] + 90, 1, 0, 0);
	drawArm2();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.0);
	glRotatef(arm3[0], 0, 0, 1);
	glRotatef(arm3[1] + 90, 1, 0, 0);
	drawArm3();
	glTranslatef(0.0, 0.0, 0.7);
	drawClaw();
	if (robot != NULL && showAttachment == true) {
		glTranslatef(0.0, -0.05, 0.4);
		glScalef(1.5, 1.5, 1.5);
		(*robot).draw();
		glScalef(1.0 / 1.5, 1.0 / 1.5, 1.0 / 1.5);
		glTranslatef(0.0, 0.05, -0.4);
	}
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glTranslatef(0.0, -0.2, 0.0);
	glColor3f(1.0, 1.0, 1.0);	// »Ö¸´°×É«
	glPopMatrix();
}

void Arm::activate() {
	state = 1;
}


void Arm::attach(Robot* rbt) {
	robot = rbt;
}

int Arm::getState() {
	return state;
}

Point Arm::getPosition() {
	return Point(x, y);
}

int Arm::getDirection() {
	return from;
}

Robot* Arm::getAttached()
{
	return robot;
}

void Arm::updateItem() {
	Map::MapUnit mu = map.getMap(tx, ty);
	if (MAP_ISBELT(mu.type))
		((Belt*)(mu.obj))->addComponent(robot, mu.i);
	if (mu.type == MAP_BOX) ((Box*)(mu.obj))->attach(robot);
}

void Arm::setColor(int c) {
	color = c;
}

void Arm::setFrom(int f) {
	from = f;
	arm1[0] = from * 90;
}

void Arm::setTo(int t) {
	to = t;
	switch (t)
	{
	case 0: {tx = x + 1; ty = y; break; }
	case 1: {tx = x; ty = y + 1; break; }
	case 2: {tx = x - 1; ty = y; break; }
	case 3: {tx = x; ty = y - 1; break; }
	default: break;
	}
}

void Arm::mySphere(float R) {
	for (int i = 0; i < 40; i++) {
		float y = 0.95 * R;
		float r = sqrt(R * R - y * y);
		float nx = r * cos(i * 18.0 * PI / 360.0);
		float ny = y;
		float nz = r * sin(i * 18.0 * PI / 360.0);
		glNormal3f(nx, ny, nz);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0, R, 0.0);
		glVertex3f(r * cos(i * 18.0 * PI / 360.0), y, r * sin(i * 18.0 * PI / 360.0));
		glVertex3f(r * cos((i + 1) * 18.0 * PI / 360.0), y, r * sin((i + 1) * 18.0 * PI / 360.0));
		glEnd();
	}
	for (int j = 19; j > -20; j--) {
		float yh = 0.05 * j * R;
		float rh = sqrt(R * R - yh * yh);
		float yl = 0.05 * (j - 1) * R;
		float rl = sqrt(R * R - yl * yl);
		for (int i = 0; i < 40; i++) {
			glNormal3f(rh * cos(i * 18.0 * PI / 360.0), yh, rh * sin(i * 18.0 * PI / 360.0));
			glBegin(GL_POLYGON);
			glVertex3f(rh * cos(i * 18.0 * PI / 360.0), yh, rh * sin(i * 18.0 * PI / 360.0));
			glVertex3f(rh * cos((i + 1) * 18.0 * PI / 360.0), yh, rh * sin((i + 1) * 18.0 * PI / 360.0));
			glVertex3f(rl * cos((i + 1) * 18.0 * PI / 360.0), yl, rl * sin((i + 1) * 18.0 * PI / 360.0));
			glVertex3f(rl * cos(i * 18.0 * PI / 360.0), yl, rl * sin(i * 18.0 * PI / 360.0));
			glEnd();
		}
	}
	for (int i = 0; i < 40; i++) {
		float y = 0.95 * (-R);
		float r = sqrt(R * R - y * y);
		glNormal3f(r * cos(i * 18.0 * PI / 360.0), y, r * sin(i * 18.0 * PI / 360.0));
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0, -R, 0.0);
		glVertex3f(r * cos(i * 18.0 * PI / 360.0), y, r * sin(i * 18.0 * PI / 360.0));
		glVertex3f(r * cos((i + 1) * 18.0 * PI / 360.0), y, r * sin((i + 1) * 18.0 * PI / 360.0));
		glEnd();
	}
}

void Arm::myCylinder(float baseR, float topR, float height) {
	for (int j = 0; j < 20; j++) {
		float rl = baseR - j * (baseR - topR) / 20;
		float yl = j * height / 20;
		float rh = baseR - (j + 1) * (baseR - topR) / 20;
		float yh = (j + 1) * height / 20;
		for (int i = 0; i < 40; i++) {
			glNormal3f(rh * cos(i * 18.0 * PI / 360.0), rh * sin(i * 18.0 * PI / 360.0), 0);
			glBegin(GL_POLYGON);
			glVertex3f(rh * cos(i * 18.0 * PI / 360.0), rh * sin(i * 18.0 * PI / 360.0), yh);
			glVertex3f(rh * cos((i + 1) * 18.0 * PI / 360.0), rh * sin((i + 1) * 18.0 * PI / 360.0), yh);
			glVertex3f(rl * cos((i + 1) * 18.0 * PI / 360.0), rl * sin((i + 1) * 18.0 * PI / 360.0), yl);
			glVertex3f(rl * cos(i * 18.0 * PI / 360.0), rl * sin(i * 18.0 * PI / 360.0), yl);
			glEnd();
		}
	}
}