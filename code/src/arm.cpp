#include "arm.h"
#include <math.h>
#define PI 3.1415926
#define L 2.0

float Arm::arm1[] = { 0.0, -20.0 };
float Arm::arm2[] = { 0.0, -35.0 };
float Arm::arm3[] = { 0.0, -15.0 };
float Arm::clawAngle = 45.0;
const float Arm::c = PI / 180;

Arm::Arm() = default;

void Arm::init() {

}

void Arm::update(int comp, int type, float deg) {
	if (comp == 1) {
		arm1[type] += deg;
		if (arm1[0] > 180) arm1[0] = 179.9;
		if (arm1[0] < -180) arm1[0] = -179.9;
		if (arm1[1] > 15.0) arm1[1] = 14.9;
		if (arm1[1] < -30.0) arm1[1] = -29.9;
		//printf("update(%d, %d, %.1f); %.1f\n", comp, type, deg, arm1[type]);
	}
	if (comp == 2) {
		arm2[type] += deg;
		if (arm2[0] > 60.0) arm2[0] = 59.9;
		if (arm2[0] < -60.0) arm2[0] = -59.9;
		if (arm2[1] > 0.0) arm2[1] = -0.1;
		if (arm2[1] < -50.0) arm2[1] = -49.9;
		//printf("update(%d, %d, %.1f); %.1f\n", comp, type, deg, arm2[type]);
	}
	if (comp == 3) {
		arm3[type] += deg;
		if (arm3[0] > 60.0) arm3[0] = 59.9;
		if (arm3[0] < -60.0) arm3[0] = -59.9;
		if (arm3[1] > 10.0) arm3[1] = 9.9;
		if (arm3[1] < -45.0) arm3[1] = -44.9;
		//printf("update(%d, %d, %.1f); %.1f\n", comp, type, deg, arm3[type]);
	}
	if (comp == 4) {
		clawAngle += deg;
		if (clawAngle > 70.0) clawAngle = 69.9;
		if (clawAngle < 30.0) clawAngle = 30.1;
		//printf("update(%d, %d, %.1f); %.1f\n", comp, type, deg, clawAngle);
	}
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
	glPushMatrix();
		glScalef(0.4, 0.4, 0.4);
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
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}