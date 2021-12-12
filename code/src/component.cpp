#include"component.h" 
#include"gl/glut.h"
robot::robot()
{
	head_r = 1.7; head_x = 0; head_y = 0.8; head_z = 0; head_t = 2; 
	body_x = 0; body_y = -3.7; body_z = 0; body_l = 5; body_w = 5.4; body_h = 3; body_mul = 1; body_t = 1;
	arm_x = -4; arm_y = -2; arm_z = 0; arm_mul = 1;
	leg_x = -1; leg_y = -8.4; leg_z = 0; leg_mul = 1;
}

void robot::headtranslate(double x,double y,double z)
{
	head_x = x; head_y = y; head_z = z;
};

void robot::bodytranslate(double x, double y, double z)
{
	body_x = x; body_y = y; body_z = z;
}

void robot::armtranslate(double x, double y, double z)
{
	arm_x = x; arm_y = y; arm_z = z;
};

void robot::legtranslate(double x, double y, double z)
{
	leg_x = x; leg_y = y; leg_z = z;
};

void robot::drawhead()
{
	if (head_t == 1) drawhead1();
	else if (head_t == 2) drawhead2();
};

void robot::drawbody()
{
	if (body_t == 1) drawbody1();
	else if (body_t == 2) drawbody2();
};


void robot::drawhead1()
{
	GLUquadric* pObj;
	pObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(head_x, head_y, head_z);
	glutSolidSphere(head_r,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.87*head_r,0.8*head_r,0);
	glRotatef(35, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r*0.2, 0, head_r*0.9, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.87 * head_r, 0.8 * head_r, 0);
	glRotatef(145, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gluCylinder(pObj, head_r * 0.3, 0, head_r * 0.9, 16, 16);
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
			y1 = x * x - 0.1;
			y2 = 0.4 * x * x + 0.3;
			glVertex3d(x, y1, z);
			glVertex3d(x, y2, z);
		}
		glEnd();
	}
	
};

void robot::drawhead2()
{
	glPushMatrix();
	glTranslated(head_x, head_y, head_z);
	glScaled(head_r*1.8, head_r*1.5, head_r);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(head_x, head_y, head_z);
	glTranslated(-1.05*head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r*0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(head_x, head_y, head_z);
	glTranslated(1.05 * head_r, 0, 0);
	glScaled(head_r * 0.35, head_r * 0.7, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(head_x, head_y, head_z);
	glTranslated(0, 0.88*head_r, 0);
	glScaled(head_r, head_r * 0.25, head_r * 0.5);
	glutSolidCube(1);
	glPopMatrix();
};

void robot::drawbody1()
{
	glPushMatrix();
	glTranslated(body_x, body_y, body_z);
	glScalef(body_l, body_w, body_h);
	glutSolidCube(1);
	glPopMatrix();
}

void robot::drawbody2()
{
	double x1=0,x2=0,y1=-0.5*body_w,y2=0,z1=0,z2=0,r=0.5*body_l;

	glPushMatrix();
	glTranslated(body_x, body_y, body_z);
	for (int i = 1; i <= 100; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		r = r + 0.01 * body_l;
		y1 = y1 + body_w / 100;
		y2 = y1 + body_w / 100;
		for (int j = 0; j <= 360; j++)
		{
			x1 = body_l * 0.5 * sin(j);
			z1 = sqrt(r*r - x1*x1);
			if (j > 180) z1 = 0 - z1;
			x2 = body_l * 0.5 * sin(j);
			z2 = sqrt(r*r - x2 * x2);
			if (j > 180) z2 = 0 - z2;
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
	
	glPopMatrix();
}

void robot::drawarm()
{
	glPushMatrix();
	glTranslated(arm_x,arm_y,arm_z);
	GLdouble eqn[4] = { 0.0, 1.0, 0.0, 0.0 };
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glutSolidSphere(1*arm_mul,20,20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(arm_x, arm_y-1.5*arm_mul, arm_z);
	glScalef(1*arm_mul, 3*arm_mul, 1*arm_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(arm_x, arm_y-3.5*arm_mul, arm_z);
	glutSolidSphere(arm_mul*1, 20, 20);
	glPopMatrix();
};

void robot::drawleg()
{
	glPushMatrix();
	glTranslated(leg_x, leg_y, leg_z);
	glScaled(1.2*leg_mul, 3.5*leg_mul, 1.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(leg_x, leg_y-1.3*leg_mul, leg_z);
	glScaled(1.5*leg_mul, 1*leg_mul, 2.5*leg_mul);
	glutSolidCube(1);
	glPopMatrix();
};
