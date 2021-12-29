#include"box.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

TexLoader Box::t(2);

void Box::init()
{
	x = 0.2; y = 0.15; z = 0.2; 
	tran_x = 1;  tran_y = 0.5; tran_z = -1; 
	tran_y2 = 0; rotate = 0; frotate = 360;
	r_typ = 1; rec_flag = false;
	t.genTex();
	t.loadTex(0, "textures/iron.bmp");
	t.loadTex(1, "textures/belt.bmp");
	rneed = new Robot(1);
	r = new Robot(1);
	setType(2);
}

Box::Box() = default;

bool Box::getReady()
{
	return ready;
}

void Box::setRType(int rr_typ)
{
	r->setType(rr_typ);
	r_typ = rr_typ;
}

int Box::getRType()
{
	return r->getType();
}

Robot* Box::getAttached()
{
	has_r = 0;
	return r;
}

void Box::addRobot()  //����Լ��������
{
	r = new Robot(r_typ);
	has_r = 1;
}

void Box::attach(Robot* robot)
{
	r = robot;
	has_r = 1;
	r_typ = r->getType();
}

void Box::deleteRobot()
{
	has_r = 0;
}

void Box::setType(int b_typ)
{
	typ = b_typ;
	if (typ == 1)  //���
	{
		for (int i = 0; i <= 5; i++)
			need[i] = 9;
		addRobot();
	}
	else
	{
		srand((unsigned)time(NULL));
		need[0] = rand()%2+1;
		need[1] = rand()%2+3;
		need[2] = rand()%2+5;
		need[3] = need[2];
		need[4] = rand()%2+7;
		need[5] = need[4];
		for (int i = 0; i < 6; i++)
			rec[i] = 0;
		crect = 0;
		if (need[5] == 8) cneedt = 5;
		else cneedt = 6;
		deleteRobot();
	}
}

void Box::receiveRobot()
{
	rec_flag = false;
	for (int i = 0; i < 6; i++)
	{
		if (need[i] == r->getType() && rec[i] == 0) 
		{
			rec[i] = need[i];
			crect++;
			rec_flag = true;
			break;
		}//�о�Ҫ�ӵ㻥�� 
	}
	deleteRobot();
	frotate = 0;
}

void Box::drawright()
{
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(tran_x,tran_y+0.1,tran_z);
	glRotatef(frotate, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.2,0.2,0);
	glVertex3f(-0.1,0.3,0);
	glVertex3f(0,0.2,0);
	glVertex3f(0,0,0);

	glVertex3f(0,0,0);
	glVertex3f(0.4,0.4,0);
	glVertex3f(0.3,0.5,0);
	glVertex3f(0,0.2,0);
	glEnd();
	glPopMatrix();
}

void Box::drawwrong()
{
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(tran_x, tran_y + 0.3, tran_z);
	glRotatef(frotate, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, 0.1, 0);
	glVertex3f(-0.1, 0.2, 0);
	glVertex3f(0.2, -0.1, 0);
	glVertex3f(0.1, -0.2, 0);

	glVertex3f(-0.2, -0.1, 0);
	glVertex3f(-0.1, -0.2, 0);
	glVertex3f(0.2, 0.1, 0);
	glVertex3f(0.1, 0.2, 0);
	glEnd();
	glPopMatrix();
}


void Box::draw()
{
	ready = (typ == 1 && has_r && tran_y2 >= 0) || (typ == 2 && !has_r && tran_y2 >= 0);
	rotate += 0.1;
	if (rotate > 360) rotate -= 360;
	frotate += 6;
	if (frotate > 720) frotate -= 360;

	glEnable(GL_TEXTURE_2D);
	t.bindTex(0);

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScalef(x, y, z);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, -1);

	glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, -1, 1);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	t.bindTex(1);

	glPushMatrix();
	glTranslatef(tran_x, tran_y+tran_y2, tran_z);
	glScalef(x, y, z);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);  glVertex3f(-0.8, 1, 0.8);
	glTexCoord2f(1, 0);  glVertex3f(-0.8, 1, -0.8);
	glTexCoord2f(0, 0);  glVertex3f(0.8, 1, -0.8);
	glTexCoord2f(0, 1);  glVertex3f(0.8, 1, 0.8);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	if (typ == 1)
	{
		if (has_r == 1)
		{
			glPushMatrix();
			glTranslatef(tran_x, tran_y +y +tran_y2,tran_z);
			r->draw();
			glPopMatrix();
			if (tran_y2 < 0) tran_y2 += 0.002;
		}
		else
		{
			if (tran_y2 < -y * 1.4)
			{
				addRobot();
			}
			else tran_y2 -= 0.002;
		}
	}//���

	if (typ == 2)
	{
		drawWholeRobot();
		if (has_r == 1)
		{
			glPushMatrix();
			glTranslatef(tran_x, tran_y + y + tran_y2, tran_z);
			r->draw();
			glPopMatrix();
			if (tran_y2 < -y * 1.4)
			{
				receiveRobot();
			}
			else tran_y2 -= 0.002;
		}
		else
		{
			if (tran_y2 < 0) tran_y2 += 0.002;
		}
	}

	if (frotate < 360)
		if (rec_flag) drawright();
		else drawwrong();

	if (cneedt == crect)
	{
		glPushMatrix();
		glTranslated(0, -0.2, 0);
		drawWholeRobot();
		glPopMatrix();
	}
}

void Box::drawWholeRobot()  
{
	glColor3d(1.0,1.0,1.0);
	rneed->setType(need[0]);
	glPushMatrix();
	glTranslatef(tran_x , tran_y+0.8, tran_z);
	glRotated(rotate, 0, 1, 0);
	rneed->draw();
	glPopMatrix();

	rneed->setType(need[1]);
	glPushMatrix();
	glTranslatef(tran_x , tran_y+0.62, tran_z);
	glRotated(rotate, 0, 1, 0);
	rneed->draw();
	glPopMatrix();

	rneed->setType(need[2]);
	glPushMatrix();
	glTranslatef(tran_x, tran_y+0.7, tran_z);
	glRotated(rotate, 0, 1, 0);
	glTranslatef(0.15, 0, 0);
	rneed->draw();
	glPopMatrix();

	rneed->setType(need[3]);
	glPushMatrix();
    glTranslatef(tran_x, tran_y+0.7, tran_z);
	glRotated(rotate, 0, 1, 0);
	glTranslatef(-0.15, 0, 0);
	rneed->draw();
	glPopMatrix();

	if (need[4] != 8)
	{
		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x, tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		glTranslatef(0.08, 0, 0);
		rneed->draw();
		glPopMatrix();

		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x , tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		glTranslatef(-0.08, 0, 0);
		rneed->draw();
		glPopMatrix();
	}
	else
	{
		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x, tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		rneed->draw();
		glPopMatrix();
	}
	
}
