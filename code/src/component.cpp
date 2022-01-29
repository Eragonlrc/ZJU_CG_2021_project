#include"component.h"
#include<stdio.h>
#include"obj.h"
extern GLuint objindex;

Robot::Robot(int _type)
{
	head_r = 1.7; body_l = 5; body_w = 5.4; body_h = 3; type = _type;
	tran_x = 0; tran_y = 0; tran_z = 0;
	arm_mul = 1; leg_mul = 1;
}

int Robot::getType()
{
	return type;
};

void Robot::setType(int t)
{
	type = t;
}

void Robot::draw()
{
	glPushMatrix();
	glTranslatef(0, 0.07, 0);
	//glScalef(0.04, 0.04, 0.04);	// change the size to fit the belt
	
	if (type == 1) drawHead1();
	if (type == 2) drawHead2();
	
	if (type == 3) drawBody1();
	if (type == 4) drawBody2();
	
	if (type == 5) drawArm1();
	if (type == 6) drawArm2();
	
	if (type == 7) drawLeg1();
	if (type == 8) drawLeg2();
	glPopMatrix();
}

void Robot::setTranslate(double x,double y,double z)
{
	tran_x = x; tran_y = y; tran_z = z;
}

void Robot::drawHead1()
{
	/*GLUquadric* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glutSolidSphere(head_r,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(0.8*head_r,0.58*head_r,0);
	glRotatef(35, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r*0.2, 0, head_r*0.9, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(-0.8 * head_r, 0.58 * head_r, 0);
	glRotatef(145, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r * 0.2, 0, head_r * 0.9, 16, 16);
	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glRotatef(90, 0, 1, 0);
	glCallList(objindex);
	glPopMatrix();
};

void Robot::drawHead2()
{
	/*glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScaled(head_r*1.8, head_r*1.5, head_r);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(-1.05*head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r*0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(1.05 * head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(0, 0.88*head_r, 0);
	glScaled(head_r, head_r * 0.25, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glRotated(90, 0, 1, 0);
	glCallList(objindex + 1);
	glPopMatrix();
};

void Robot::drawBody1()
{
	/*glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScalef(body_l, body_w, body_h);
	glutSolidCube(1);
	glPopMatrix();*/

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glRotatef(90, 0, 1, 0);
	glCallList(objindex + 2);
	glPopMatrix();
}

void Robot::drawBody2()
{
	/*glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);

	glPushMatrix();
	glScaled(body_l, 0.8*body_l, body_l);
	glutSolidSphere(0.6, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0.5*body_l,0);
	glScaled(body_l, 0.7 * body_l, body_l);
	glutSolidSphere(0.4, 30, 30);
	glPopMatrix();

	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glCallList(objindex + 3);
	glPopMatrix();
}

void Robot::drawArm1()
{
	/*glPushMatrix();
	glTranslatef(tran_x,tran_y,tran_z);
	GLdouble eqn[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(1*arm_mul,20,20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y-1.5*arm_mul, tran_z);
	glScalef(1*arm_mul, 3*arm_mul, 1*arm_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y-3.5*arm_mul, tran_z);
	glutSolidSphere(1*arm_mul, 20, 20);
	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glCallList(objindex + 4);
	glPopMatrix();
};

void Robot::drawArm2()
{
	/*glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	GLdouble eqn[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.8 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	GLUquadric* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(tran_x, tran_y - 3 * arm_mul, tran_z);
	glRotated(270, 1, 0, 0);
	gluCylinder(pObj, 1.1*arm_mul, 0.8*arm_mul, 3*arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y - 3 * arm_mul, tran_z);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(1.1 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//
	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(-2.3 * arm_mul, -1.2 * arm_mul, 0);
	glRotatef(50, 0, 0, 1);
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0,  - 4 * arm_mul, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * arm_mul, 0.3 * arm_mul, 0.7 * arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,  - 4.7 * arm_mul, 0);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.3 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPopMatrix();
	//

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glTranslatef(2.3 * arm_mul, -1.2 * arm_mul, 0);
	glRotatef(310, 0, 0, 1);
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0, -4 * arm_mul, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * arm_mul, 0.3 * arm_mul, 0.7 * arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -4.7 * arm_mul, 0);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.3 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y+0.07, tran_z);
	glCallList(objindex + 5);
	glPopMatrix();
};

void Robot::drawLeg1()
{
	/*glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScaled(1.2*leg_mul, 3.5*leg_mul, 1.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tran_x, tran_y-1.3*leg_mul, tran_z);
	glScaled(1.5*leg_mul, 1*leg_mul, 2.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();*/
	
	glPushMatrix();
	glTranslatef(tran_x, tran_y - 0.07, tran_z);
	glCallList(objindex + 6);
	glPopMatrix();
};

void Robot::drawLeg2()
{
	/*glPushMatrix();
	glTranslatef(tran_x,tran_y,tran_z);
	GLUquadric* pObj;

	pObj = gluNewQuadric();
	glPushMatrix();
	glRotated(270, 1, 0, 0);
	gluCylinder(pObj, 0.2 * leg_mul, 0.2 * leg_mul, 2.5 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(-3*leg_mul,0,0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * leg_mul, 0.2 * leg_mul, 6 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(-3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 1.5 * leg_mul, 1.5 * leg_mul, 0.7 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(3 * leg_mul, 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 1.5 * leg_mul, 1.5 * leg_mul, 0.7 * leg_mul, 16, 16);
	glPopMatrix();

	float R = 1.5*leg_mul;
	int n = 100;     
	glPushMatrix();
	glTranslatef(3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)     
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.1 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();*/

	glPushMatrix();
	glTranslatef(tran_x, tran_y+0.04, tran_z);
	glCallList(objindex + 7);
	glPopMatrix();
};
