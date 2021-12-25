#include "arm.h"
#include <math.h>

Arm::Arm(int dx, int dy, int f, int t) {
	x = dx;
	y = dy;
	state = 0;
	from = f;
	to = t;
	phase = 0;
	arm1[0] = f * 90.0;
	arm1[1] = -20.0;
	arm2[0] = 0.0;
	arm2[1] = -35.0;
	arm3[0] = 0.0;
	arm3[1] = -15.0;
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
}

void Arm::update() {
	/*
		state	operation
		0		ready(reset)
		1		bend
		2		fetch
		3		lift
		4		rotate
		5		release
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
		else if (abs(arm3[1] - (-15.0)) > 0.05) arm3[1] += 0.2;
		else state = 4;
	}
	if (state == 4) {
		if (abs(arm1[0] - to * 90) > 0.5 || (to == 0 && (arm1[0] < 359.0 && arm1[0] > 1.0)))	arm1[0] -= clockWise * 0.5;
		else state = 1;
	}
	if (state == 5) {
		if (clawAngle < 60.0) clawAngle += 1.0;
		else {
			state = 6;
			showAttachment = false;
		}
	}
	if (state == 6) {
		if (abs(arm1[1] - (-20.0)) > 0.05)	arm1[1] -= 0.2;
		else if (abs(arm2[1] - (-35.0)) > 0.05) arm2[1] -= 0.2;
		else if (abs(arm3[1] - (-15.0)) > 0.05) arm3[1] += 0.2;
		if (abs(arm1[0] - from * 90) > 1.0 || (from == 0 && (arm1[0] < 359.0 && arm1[0] > 1.0)))	arm1[0] += clockWise * 0.5;
		if (!(abs(arm1[0] - from * 90) > 1.0 || (from == 0 && (arm1[0] < 359.0 && arm1[0] > 1.0)))&&(!(abs(arm3[1] - (-15.0)) > 0.05))) state = 0;
	}
	if (arm1[0] > 360.0)	arm1[0] -= 360.0;
	if (arm1[0] < 0)	arm1[0] += 360.0;
	if (arm2[0] > 360.0)	arm2[0] -= 360.0;
	if (arm3[0] > 360.0)	arm3[0] -= 360.0;
}

void Arm::DrawFoundation() {
	glScalef(1.0, 0.4, 1.0);
	glutSolidCube(0.7);
	glScalef(1.0, 2.5, 1.0);
}

void Arm::DrawJoint1() {
	glTranslatef(0.0, 0.15, 0.0);
	glutSolidSphere(0.3, 20, 20);
	glTranslatef(0.0, -0.15, 0.0);
}

void Arm::DrawArm1() {
	GLUquadricObj* objCylinder = gluNewQuadric();
	gluCylinder(objCylinder, 0.23, 0.18, 1.2, 100, 100);
}

void Arm::DrawArm2() {
	glutSolidSphere(0.27, 20, 20);
	GLUquadricObj* objCylinder = gluNewQuadric();
	gluCylinder(objCylinder, 0.18, 0.15, 1.0, 100, 100);
}

void Arm::DrawArm3() {
	glutSolidSphere(0.25, 20, 20);
	GLUquadricObj* objCylinder = gluNewQuadric();
	gluCylinder(objCylinder, 0.18, 0.12, 0.7, 100, 100);
}

void Arm::DrawClaw() {
	glutSolidSphere(0.12, 20, 20);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glRotatef(120 * i, 0, 0, 1);
		glRotatef(clawAngle, 0, -1, 0);
		GLUquadricObj* objCylinder = gluNewQuadric();
		gluCylinder(objCylinder, 0.04, 0.02, 0.3, 100, 100);
		glTranslatef(0.0, 0.0, 0.3);
		glutSolidSphere(0.02, 20, 20);
		glRotatef(45, 0, 1, 0);
		gluCylinder(objCylinder, 0.02, 0.015, 0.2, 100, 100);
		glTranslatef(0.0, 0.0, 0.2);
		glutSolidSphere(0.015, 20, 20);
		glPopMatrix();
	}
}

void Arm::Draw() {
	update();
	glPushMatrix();
		glTranslatef(0.0, 0.2, 0.0);
		glScalef(0.6, 0.6, 0.6);
		DrawFoundation();
		glPushMatrix();
			glRotatef(arm1[0], 0, 1, 0);
			DrawJoint1();
			glPushMatrix();
				glTranslatef(0.0, 0.1, 0.0);
				glRotatef(arm1[1] - 90, 1, 0, 0);
				DrawArm1();
				glPushMatrix();
					glTranslatef(0.0, 0.0, 1.2);
					glRotatef(arm2[0], 0, 0, 1);
					glRotatef(arm2[1] + 90, 1, 0, 0);
					DrawArm2();
					glPushMatrix();
						glTranslatef(0.0, 0.0, 1.0);
						glRotatef(arm3[0], 0, 0, 1);
						glRotatef(arm3[1] + 90, 1, 0, 0);
						DrawArm3();
						glTranslatef(0.0, 0.0, 0.7);
						DrawClaw();
						if (robot != NULL && showAttachment == true) {
							glTranslatef(0.0, -0.05, 0.4);
							(*robot).draw();
							glTranslatef(0.0, 0.05, -0.4);
						}
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		glTranslatef(0.0, -0.2, 0.0);
	glPopMatrix();
}

void Arm::activate() {
	state = 1;
}

void Arm::Attach(Robot* rbt){
	robot = rbt;
}

int Arm::getState() {
	return state;
}

int Arm::getDirection() {
	return from;
}

Robot* Arm::getAttached()
{
	return robot;
}
