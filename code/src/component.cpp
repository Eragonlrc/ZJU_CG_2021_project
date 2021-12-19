#include"component.h"

Robot::Robot(int _type)
{
	head_r = 1.7; body_l = 5; body_w = 5.4; body_h = 3; type = _type;
	tran_x = 0; tran_y = 0; tran_z = 0;
	arm_mul = 1; leg_mul = 1;
}

int Robot::gettype()
{
	return type;
};

void Robot::settype(int t)
{
	type = t;
}

void Robot::draw()
{
	glPushMatrix();
	glTranslatef(0, 0.05, 0);
	glScalef(0.04, 0.04, 0.04);	// change the size to fit the belt
	
	if (type == 1) drawhead1();
	if (type == 2) drawhead2();
	
	if (type == 3) drawbody1();
	if (type == 4) drawbody2();
	
	if (type == 5) drawarm1();
	if (type == 6) drawarm2();
	
	if (type == 7) drawleg1();
	if (type == 8) drawleg2();
	glPopMatrix();
}

void Robot::settranslate(double x,double y,double z)
{
	tran_x = x; tran_y = y; tran_z = z;
}

void Robot::drawhead1()
{
	GLUquadric* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glutSolidSphere(head_r,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(0.8*head_r,0.58*head_r,0);
	glRotatef(35, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r*0.2, 0, head_r*0.9, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(-0.8 * head_r, 0.58 * head_r, 0);
	glRotatef(145, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r * 0.2, 0, head_r * 0.9, 16, 16);
	glPopMatrix();

	double l = head_r / 20, x = 0, y1 = 0, y2 = 0, z = 0.9*head_r;
	for (int j = 0; j <= 100; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		z = z + (head_r*0.1 / 100);
		x = -0.5 * head_r;
		for (int leng = 1; leng <= 100; leng++)
		{
			x = x + (head_r / 100);
			y1 = x * x - 0.4;
			y2 = 0.4 * x * x + 0.1;
			glVertex3d(x+tran_x, y1+tran_y, z+tran_z);
			glVertex3d(x+tran_x, y2+tran_y, z+tran_z);
		}
		glEnd();
	}
	
};

void Robot::drawhead2()
{
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glScaled(head_r*1.8, head_r*1.5, head_r);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(-1.05*head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r*0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(1.05 * head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(0, 0.88*head_r, 0);
	glScaled(head_r, head_r * 0.25, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();
};

void Robot::drawbody1()
{
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glScalef(body_l, body_w, body_h);
	glutSolidCube(1);
	glPopMatrix();
}

void Robot::drawbody2()
{
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);

	glPushMatrix();
	glScaled(body_l, 0.8*body_l, body_l);
	glutSolidSphere(0.5, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0.35*body_l,0);
	glScaled(body_l, 0.7 * body_l, body_l);
	glutSolidSphere(0.35, 30, 30);
	glPopMatrix();

	glPopMatrix();
}

void Robot::drawarm1()
{
	glPushMatrix();
	glTranslated(tran_x,tran_y,tran_z);
	GLdouble eqn[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(1*arm_mul,20,20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y-1.5*arm_mul, tran_z);
	glScalef(1*arm_mul, 3*arm_mul, 1*arm_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y-3.5*arm_mul, tran_z);
	glutSolidSphere(1*arm_mul, 20, 20);
	glPopMatrix();
};

void Robot::drawarm2()
{
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	GLdouble eqn[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.8 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	GLUquadric* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(tran_x, tran_y - 3 * arm_mul, tran_z);
	glRotated(270, 1, 0, 0);
	gluCylinder(pObj, 1.1*arm_mul, 0.8*arm_mul, 3*arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y - 3 * arm_mul, tran_z);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(1.1 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(-2.3 * arm_mul, -1.2 * arm_mul, 0);
	glRotatef(50, 0, 0, 1);
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(0,  - 4 * arm_mul, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * arm_mul, 0.3 * arm_mul, 0.7 * arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,  - 4.7 * arm_mul, 0);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.3 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPopMatrix();
	//

	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glTranslated(2.3 * arm_mul, -1.2 * arm_mul, 0);
	glRotatef(310, 0, 0, 1);
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(0, -4 * arm_mul, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * arm_mul, 0.3 * arm_mul, 0.7 * arm_mul, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -4.7 * arm_mul, 0);
	eqn[1] = -1;
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(0.3 * arm_mul, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPopMatrix();

};

void Robot::drawleg1()
{
	glPushMatrix();
	glTranslated(tran_x, tran_y, tran_z);
	glScaled(1.2*leg_mul, 3.5*leg_mul, 1.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(tran_x, tran_y-1.3*leg_mul, tran_z);
	glScaled(1.5*leg_mul, 1*leg_mul, 2.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();
};

void Robot::drawleg2()
{
	glPushMatrix();
	glTranslated(tran_x,tran_y,tran_z);
	GLUquadric* pObj;

	pObj = gluNewQuadric();
	glPushMatrix();
	glRotated(270, 1, 0, 0);
	gluCylinder(pObj, 0.2 * leg_mul, 0.2 * leg_mul, 2.5 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(-3*leg_mul,0,0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 0.2 * leg_mul, 0.2 * leg_mul, 6 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(-3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 1.5 * leg_mul, 1.5 * leg_mul, 0.7 * leg_mul, 16, 16);
	glPopMatrix();

	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(3 * leg_mul, 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(pObj, 1.5 * leg_mul, 1.5 * leg_mul, 0.7 * leg_mul, 16, 16);
	glPopMatrix();

	float R = 1.5*leg_mul;
	int n = 100;     
	glPushMatrix();
	glTranslated(3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)     
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(3.1 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3.7 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3 * leg_mul, 0, 0);
	glRotated(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
	{
		glVertex2f(R * cos(2 * 3.14 * i / n), R * sin(2 * 3.14 * i / n));
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();
};
